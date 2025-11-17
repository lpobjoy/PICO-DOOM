/**
 * Display adapter for PICO-DOOM
 * Bridges PicoDoom rendering to Pimoroni ST7789 displays
 */

#ifndef DISPLAY_ADAPTER_H
#define DISPLAY_ADAPTER_H

#include <stdint.h>
#include <stdbool.h>

// Display configuration
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define DOOM_WIDTH     320
#define DOOM_HEIGHT    200

// Color format: RGB565
typedef uint16_t pixel_t;

// Frame buffer type
typedef struct {
    pixel_t *data;
    uint16_t width;
    uint16_t height;
    volatile bool ready;
} framebuffer_t;

/**
 * Initialize the display system
 * Sets up SPI, display driver, and framebuffers
 */
void display_init(void);

/**
 * Get the current framebuffer for rendering
 * Returns a pointer to the active drawing buffer
 */
framebuffer_t* display_get_framebuffer(void);

/**
 * Swap framebuffers (double buffering)
 * Presents the completed frame to the display
 */
void display_swap_buffers(void);

/**
 * Wait for display to be ready for next frame
 */
void display_wait_vsync(void);

/**
 * Convert 8-bit palette index to RGB565
 * doom_palette: pointer to Doom's 256-color palette (RGB888)
 * index: palette index (0-255)
 * returns: RGB565 color value
 */
pixel_t display_palette_to_rgb565(const uint8_t *doom_palette, uint8_t index);

/**
 * Clear the screen to a specific color
 */
void display_clear(pixel_t color);

/**
 * Update display brightness (0-255)
 */
void display_set_brightness(uint8_t brightness);

/**
 * Get FPS counter
 */
float display_get_fps(void);

/**
 * Core 1 display update loop
 * Continuously pushes framebuffer to display
 */
void display_core1_loop(void);

#endif // DISPLAY_ADAPTER_H
