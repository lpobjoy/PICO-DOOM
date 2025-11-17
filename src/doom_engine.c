/**
 * Doom engine wrapper implementation
 * Stub for now - full integration will be done in phases
 */

#include "doom_engine.h"
#include "display_adapter.h"
#include "wad_loader.h"
#include <stdio.h>
#include <string.h>

// Global game state
static bool doom_initialized = false;
static uint32_t frame_count = 0;
static int test_pattern_mode = 0;  // 0=color bars, 1=checkerboard, 2=gradient
static wad_file_t *loaded_wad = NULL;

// Simple 8-bit Doom palette (first 16 colors for test)
// Format: R, G, B for each color
static const uint8_t test_palette[] = {
    0x00, 0x00, 0x00,  // 0: Black
    0xFF, 0x00, 0x00,  // 1: Red
    0x00, 0xFF, 0x00,  // 2: Green
    0x00, 0x00, 0xFF,  // 3: Blue
    0xFF, 0xFF, 0x00,  // 4: Yellow
    0xFF, 0x00, 0xFF,  // 5: Magenta
    0x00, 0xFF, 0xFF,  // 6: Cyan
    0xFF, 0xFF, 0xFF,  // 7: White
    0x80, 0x80, 0x80,  // 8: Gray
    0xFF, 0x80, 0x00,  // 9: Orange
    0x80, 0x00, 0x80,  // 10: Purple
    0x00, 0x80, 0x80,  // 11: Teal
    0x80, 0xFF, 0x00,  // 12: Lime
    0xFF, 0x00, 0x80,  // 13: Pink
    0x00, 0x80, 0xFF,  // 14: Sky
    0x80, 0x80, 0xFF,  // 15: Lavender
};

/**
 * Convert RGB888 to RGB565
 */
static inline uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
    // RGB888 -> RGB565
    // R: 8 bits -> 5 bits (shift right 3)
    // G: 8 bits -> 6 bits (shift right 2)
    // B: 8 bits -> 5 bits (shift right 3)
    uint16_t rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
    return __builtin_bswap16(rgb565);  // Byte swap for ST7789
}

bool doom_init(void) {
    printf("Initializing Doom engine...\n");
    doom_initialized = true;
    frame_count = 0;
    test_pattern_mode = 0;
    loaded_wad = NULL;
    printf("Doom engine initialized (stub mode)\n");
    printf("Test patterns available: color bars (0), checkerboard (1), gradient (2)\n");
    printf("Use weapon_up/down buttons to cycle through patterns\n");
    return true;
}

bool doom_load_wad(const char *wad_path) {
    if (!doom_initialized) {
        printf("Error: Doom engine not initialized\n");
        return false;
    }
    
    printf("Loading WAD file: %s\n", wad_path);
    
    // TODO: Implement flash/filesystem WAD loading
    // For now, just show instructions on how to provide WAD data
    printf("WAD loading not yet implemented (Phase 3)\n");
    printf("To load a WAD file:\n");
    printf("  1. Extract DOOM1.WAD contents\n");
    printf("  2. Store in flash storage\n");
    printf("  3. Call wad_load_from_memory() with the data\n");
    printf("\nUsing test rendering patterns for now\n");
    
    return false;  // Return false since we're stubbed
}

void doom_update(const doom_input_t *input) {
    if (!doom_initialized) {
        return;
    }
    
    // Check for input to change test pattern
    if (input) {
        if (input->weapon_next) {
            test_pattern_mode = (test_pattern_mode + 1) % 3;
            printf("Switched to test pattern mode: %d\n", test_pattern_mode);
        }
        if (input->weapon_prev && test_pattern_mode > 0) {
            test_pattern_mode--;
            printf("Switched to test pattern mode: %d\n", test_pattern_mode);
        }
    }
    
    // TODO: Update game logic with input
    frame_count++;
}

void doom_render(uint16_t *framebuffer) {
    if (!doom_initialized || !framebuffer) {
        return;
    }
    
    // Get display dimensions from display adapter
    framebuffer_t *fb = display_get_framebuffer();
    if (!fb || !fb->data) {
        return;
    }
    
    int width = fb->width;
    int height = fb->height;
    
    // Render test pattern based on current mode
    switch (test_pattern_mode) {
        case 0: {
            // Color bar pattern (vertical stripes)
            uint16_t colors[] = {
                rgb888_to_rgb565(test_palette[0], test_palette[1], test_palette[2]),    // Black
                rgb888_to_rgb565(test_palette[3], test_palette[4], test_palette[5]),    // Red
                rgb888_to_rgb565(test_palette[6], test_palette[7], test_palette[8]),    // Green
                rgb888_to_rgb565(test_palette[9], test_palette[10], test_palette[11]),  // Blue
                rgb888_to_rgb565(test_palette[12], test_palette[13], test_palette[14]), // Yellow
                rgb888_to_rgb565(test_palette[15], test_palette[16], test_palette[17]), // Magenta
            };
            int num_colors = sizeof(colors) / sizeof(colors[0]);
            int bar_width = width / num_colors;
            
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int color_idx = (x / bar_width) % num_colors;
                    fb->data[y * width + x] = colors[color_idx];
                }
            }
            break;
        }
        
        case 1: {
            // Checkerboard pattern
            int checker_size = 8;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int checker = ((x / checker_size) + (y / checker_size)) & 1;
                    uint16_t color = checker ? 
                        rgb888_to_rgb565(255, 255, 255) :  // White
                        rgb888_to_rgb565(0, 0, 0);          // Black
                    fb->data[y * width + x] = color;
                }
            }
            break;
        }
        
        case 2: {
            // Gradient pattern (left-right)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    uint8_t r = (x * 255) / width;
                    uint8_t g = (y * 255) / height;
                    uint8_t b = 128 + ((frame_count * 2) % 128);
                    fb->data[y * width + x] = rgb888_to_rgb565(r, g, b);
                }
            }
            break;
        }
    }
}

void doom_get_state(char *buffer, int max_len) {
    if (!buffer || max_len <= 0) {
        return;
    }
    
    const char *mode_names[] = {"Bars", "Check", "Grad"};
    const char *mode_name = (test_pattern_mode < 3) ? mode_names[test_pattern_mode] : "Unknown";
    
    snprintf(buffer, max_len, "Frame: %lu | Pattern: %s", frame_count, mode_name);
}

void doom_shutdown(void) {
    printf("Shutting down Doom engine\n");
    if (loaded_wad) {
        wad_free(loaded_wad);
        loaded_wad = NULL;
    }
    doom_initialized = false;
}
