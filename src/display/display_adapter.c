/**
 * Display adapter implementation for PICO-DOOM
 * Integrates Pimoroni ST7789 display with Doom rendering
 */

#include "display_adapter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"

// ST7789 Display pins (Pimoroni Pico Display 2.0)
#define LCD_SPI       spi0
#define LCD_SPI_SCK   18
#define LCD_SPI_MOSI  19
#define LCD_DC        16
#define LCD_CS        17
#define LCD_RESET     21
#define LCD_BL        20

// ST7789 Commands
#define ST7789_SWRESET   0x01
#define ST7789_SLPOUT    0x11
#define ST7789_NORON     0x13
#define ST7789_INVON     0x21
#define ST7789_DISPOFF   0x28
#define ST7789_DISPON    0x29
#define ST7789_CASET     0x2A
#define ST7789_RASET     0x2B
#define ST7789_RAMWR     0x2C
#define ST7789_MADCTL    0x36
#define ST7789_COLMOD    0x3A

// Double buffering
static framebuffer_t framebuffers[2];
static volatile int current_fb = 0;
static volatile int display_fb = 1;
static mutex_t fb_mutex;
static semaphore_t frame_ready;

// FPS tracking
static volatile uint32_t frame_count = 0;
static volatile uint64_t last_fps_time = 0;
static volatile float current_fps = 0.0f;

// Doom palette cache (RGB565 converted)
static pixel_t palette_cache[256];

/**
 * Write command to ST7789
 */
static inline void lcd_write_cmd(uint8_t cmd) {
    gpio_put(LCD_CS, 0);
    gpio_put(LCD_DC, 0);  // Command mode
    spi_write_blocking(LCD_SPI, &cmd, 1);
    gpio_put(LCD_CS, 1);
}

/**
 * Write data to ST7789
 */
static inline void lcd_write_data(const uint8_t *data, size_t len) {
    gpio_put(LCD_CS, 0);
    gpio_put(LCD_DC, 1);  // Data mode
    spi_write_blocking(LCD_SPI, data, len);
    gpio_put(LCD_CS, 1);
}

/**
 * Write single byte of data
 */
static inline void lcd_write_byte(uint8_t data) {
    lcd_write_data(&data, 1);
}

/**
 * Set drawing window on display
 */
static void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    lcd_write_cmd(ST7789_CASET);
    uint8_t caset[] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    lcd_write_data(caset, 4);
    
    lcd_write_cmd(ST7789_RASET);
    uint8_t raset[] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
    lcd_write_data(raset, 4);
    
    lcd_write_cmd(ST7789_RAMWR);
}

/**
 * Initialize ST7789 display
 */
static void lcd_init(void) {
    // Hardware reset
    gpio_put(LCD_RESET, 1);
    sleep_ms(5);
    gpio_put(LCD_RESET, 0);
    sleep_ms(20);
    gpio_put(LCD_RESET, 1);
    sleep_ms(150);
    
    // Software reset
    lcd_write_cmd(ST7789_SWRESET);
    sleep_ms(150);
    
    // Exit sleep mode
    lcd_write_cmd(ST7789_SLPOUT);
    sleep_ms(10);
    
    // Set color mode to 16-bit (RGB565)
    lcd_write_cmd(ST7789_COLMOD);
    lcd_write_byte(0x55);  // 16-bit color
    
    // Memory access control (screen orientation)
    lcd_write_cmd(ST7789_MADCTL);
    lcd_write_byte(0x00);  // Normal orientation
    
    // Inversion on (looks better)
    lcd_write_cmd(ST7789_INVON);
    
    // Normal display mode
    lcd_write_cmd(ST7789_NORON);
    sleep_ms(10);
    
    // Display on
    lcd_write_cmd(ST7789_DISPON);
    sleep_ms(10);
    
    printf("ST7789 display initialized\n");
}

/**
 * Initialize display system
 */
void display_init(void) {
    printf("Initializing display adapter...\n");
    
    // Initialize SPI
    spi_init(LCD_SPI, 62500000);  // 62.5 MHz (fast!)
    gpio_set_function(LCD_SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(LCD_SPI_MOSI, GPIO_FUNC_SPI);
    
    // Initialize control pins
    gpio_init(LCD_DC);
    gpio_set_dir(LCD_DC, GPIO_OUT);
    gpio_init(LCD_CS);
    gpio_set_dir(LCD_CS, GPIO_OUT);
    gpio_put(LCD_CS, 1);
    
    gpio_init(LCD_RESET);
    gpio_set_dir(LCD_RESET, GPIO_OUT);
    
    gpio_init(LCD_BL);
    gpio_set_dir(LCD_BL, GPIO_OUT);
    gpio_put(LCD_BL, 1);  // Backlight on
    
    // Initialize framebuffers
    for (int i = 0; i < 2; i++) {
        framebuffers[i].width = DISPLAY_WIDTH;
        framebuffers[i].height = DOOM_HEIGHT;
        framebuffers[i].data = (pixel_t*)malloc(DISPLAY_WIDTH * DOOM_HEIGHT * sizeof(pixel_t));
        framebuffers[i].ready = false;
        
        if (!framebuffers[i].data) {
            printf("ERROR: Failed to allocate framebuffer %d\n", i);
            panic("Out of memory");
        }
        
        // Clear to black
        memset(framebuffers[i].data, 0, DISPLAY_WIDTH * DOOM_HEIGHT * sizeof(pixel_t));
    }
    
    // Initialize synchronization primitives
    mutex_init(&fb_mutex);
    sem_init(&frame_ready, 0, 1);
    
    // Initialize LCD
    lcd_init();
    
    printf("Display adapter initialized: %dx%d framebuffers\n", DISPLAY_WIDTH, DOOM_HEIGHT);
}

/**
 * Get current drawing framebuffer
 */
framebuffer_t* display_get_framebuffer(void) {
    return &framebuffers[current_fb];
}

/**
 * Swap framebuffers
 */
void display_swap_buffers(void) {
    mutex_enter_blocking(&fb_mutex);
    
    // Swap buffers
    int temp = current_fb;
    current_fb = display_fb;
    display_fb = temp;
    
    // Mark as ready for display
    framebuffers[display_fb].ready = true;
    
    mutex_exit(&fb_mutex);
    
    // Signal frame ready
    sem_release(&frame_ready);
}

/**
 * Wait for vsync
 */
void display_wait_vsync(void) {
    // Wait for previous frame to be displayed
    while (framebuffers[current_fb].ready) {
        tight_loop_contents();
    }
}

/**
 * Convert Doom palette to RGB565
 */
pixel_t display_palette_to_rgb565(const uint8_t *doom_palette, uint8_t index) {
    // Doom palette is RGB888 (3 bytes per color)
    const uint8_t *rgb = &doom_palette[index * 3];
    uint8_t r = rgb[0];
    uint8_t g = rgb[1];
    uint8_t b = rgb[2];
    
    // Convert to RGB565 and byte swap for ST7789
    uint16_t color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    return __builtin_bswap16(color565);
}

/**
 * Update palette cache
 */
void display_update_palette(const uint8_t *doom_palette) {
    for (int i = 0; i < 256; i++) {
        palette_cache[i] = display_palette_to_rgb565(doom_palette, i);
    }
}

/**
 * Clear screen
 */
void display_clear(pixel_t color) {
    framebuffer_t *fb = display_get_framebuffer();
    for (int i = 0; i < fb->width * fb->height; i++) {
        fb->data[i] = color;
    }
}

/**
 * Set backlight brightness (0-255)
 */
void display_set_brightness(uint8_t brightness) {
    // Simple on/off for now (could use PWM for dimming)
    gpio_put(LCD_BL, brightness > 0);
}

/**
 * Get FPS
 */
float display_get_fps(void) {
    return current_fps;
}

/**
 * Core 1 display loop - continuously updates display
 */
void display_core1_loop(void) {
    printf("Core 1: Display update loop started\n");
    
    uint64_t last_time = time_us_64();
    uint32_t local_frame_count = 0;
    
    while (true) {
        // Wait for frame to be ready
        sem_acquire_blocking(&frame_ready);
        
        mutex_enter_blocking(&fb_mutex);
        
        if (framebuffers[display_fb].ready) {
            framebuffer_t *fb = &framebuffers[display_fb];
            
            // Set display window (Doom is 320x200, center on 320x240 display)
            uint16_t y_offset = (DISPLAY_HEIGHT - DOOM_HEIGHT) / 2;
            
            // Clear top border (black bars for letterboxing)
            if (y_offset > 0) {
                lcd_set_window(0, 0, DISPLAY_WIDTH - 1, y_offset - 1);
                pixel_t black = 0;
                for (int i = 0; i < DISPLAY_WIDTH * y_offset; i++) {
                    lcd_write_data((uint8_t*)&black, 2);
                }
            }
            
            // Draw Doom frame
            lcd_set_window(0, y_offset, DISPLAY_WIDTH - 1, y_offset + DOOM_HEIGHT - 1);
            
            // Write framebuffer to display
            gpio_put(LCD_CS, 0);
            gpio_put(LCD_DC, 1);
            spi_write_blocking(LCD_SPI, (uint8_t*)fb->data, 
                             DISPLAY_WIDTH * DOOM_HEIGHT * sizeof(pixel_t));
            gpio_put(LCD_CS, 1);
            
            // Clear bottom border
            if (y_offset > 0) {
                lcd_set_window(0, y_offset + DOOM_HEIGHT, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
                pixel_t black = 0;
                for (int i = 0; i < DISPLAY_WIDTH * y_offset; i++) {
                    lcd_write_data((uint8_t*)&black, 2);
                }
            }
            
            fb->ready = false;
            local_frame_count++;
            
            // Calculate FPS every second
            uint64_t current_time = time_us_64();
            if (current_time - last_time >= 1000000) {
                current_fps = (float)local_frame_count * 1000000.0f / (float)(current_time - last_time);
                local_frame_count = 0;
                last_time = current_time;
            }
        }
        
        mutex_exit(&fb_mutex);
    }
}
