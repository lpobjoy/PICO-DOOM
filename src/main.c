/**
 * PICO-DOOM Main Entry Point
 * 
 * This is a port of Doom to the Raspberry Pi Pico using Pimoroni displays.
 * Based on PicoDoom by Graham Sanderson.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "display_adapter.h"
#include "input_handler.h"
#include "doom_engine.h"

#define LED_PIN 25

/**
 * Initialize the Pico hardware
 */
void init_hardware(void) {
    stdio_init_all();
    
    // Initialize LED for status
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    printf("PICO-DOOM Initializing...\n");
    printf("RP2040 running at %d MHz\n", clock_get_hz(clk_sys) / 1000000);
}

/**
 * Initialize the display
 */
void init_display(void) {
    printf("Initializing display...\n");
    display_init();
    display_clear(0x0000);  // Clear to black
    printf("Display ready: %dx%d\n", DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

/**
 * Initialize input system
 */
void init_input(void) {
    printf("Initializing input...\n");
    input_init();
    printf("Input system ready\n");
}

/**
 * Initialize Doom engine
 */
void init_doom(void) {
    if (!doom_init()) {
        printf("ERROR: Failed to initialize Doom engine\n");
        return;
    }
    
    // Try to load WAD file from flash
    if (!doom_load_wad("/wad/doom1.wad")) {
        printf("WARNING: Could not load WAD file\n");
    }
}

/**
 * Main game loop
 */
void game_loop(void) {
    printf("Starting game loop...\n");
    
    uint32_t frame = 0;
    uint64_t last_status = time_us_64();
    doom_input_t doom_input = {0};
    
    while (true) {
        // Update input
        input_update();
        
        // Collect input for Doom engine
        doom_input.forward = input_is_key_down(DOOM_KEY_FORWARD);
        doom_input.backward = input_is_key_down(DOOM_KEY_BACKWARD);
        doom_input.strafe_left = input_is_key_down(DOOM_KEY_STRAFE_LEFT);
        doom_input.strafe_right = input_is_key_down(DOOM_KEY_STRAFE_RIGHT);
        doom_input.turn_left = input_is_key_down(DOOM_KEY_TURN_LEFT);
        doom_input.turn_right = input_is_key_down(DOOM_KEY_TURN_RIGHT);
        doom_input.fire = input_is_key_down(DOOM_KEY_FIRE);
        doom_input.use = input_is_key_down(DOOM_KEY_USE);
        doom_input.weapon_next = input_is_key_down(DOOM_KEY_WEAPON_UP);
        doom_input.weapon_prev = input_is_key_down(DOOM_KEY_WEAPON_DOWN);
        
        // Update Doom engine
        doom_update(&doom_input);
        
        // Get framebuffer and render
        framebuffer_t *fb = display_get_framebuffer();
        doom_render(fb->data);
        
        // Swap buffers
        display_swap_buffers();
        
        // Wait for display to be ready
        display_wait_vsync();
        
        frame++;
        
        // Print status every second
        uint64_t now = time_us_64();
        if (now - last_status >= 1000000) {
            float fps = display_get_fps();
            char doom_state[64];
            doom_get_state(doom_state, sizeof(doom_state));
            printf("Frame %u | FPS: %.1f | %s | %s\n", 
                   frame, fps, input_get_debug_string(), doom_state);
            last_status = now;
            
            // Blink LED
            gpio_put(LED_PIN, !gpio_get(LED_PIN));
        }
    }
}

/**
 * Core 1 entry point (display updates)
 */
void core1_entry(void) {
    printf("Core 1 started - handling display updates\n");
    
    // Run display update loop (never returns)
    display_core1_loop();
}

/**
 * Main entry point
 */
int main(void) {
    // Initialize hardware
    init_hardware();
    
    printf("\n");
    printf("================================\n");
    printf("       PICO-DOOM v0.1\n");
    printf("================================\n");
    printf("RAM: %d KB available\n", 264);
    printf("Flash: %d MB available\n", 2);
    printf("\n");
    
    // Initialize subsystems
    init_display();
    init_input();
    init_doom();
    
    // Start rendering on Core 1
    printf("Starting Core 1 for rendering...\n");
    multicore_launch_core1(core1_entry);
    
    // Run game loop on Core 0
    game_loop();
    
    return 0;
}
