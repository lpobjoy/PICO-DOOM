/**
 * Input handler implementation for PICO-DOOM
 * Handles button inputs and maps to Doom controls
 */

#include "input_handler.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Debounce time in milliseconds
#define DEBOUNCE_MS 50

// Hold time threshold in milliseconds
#define HOLD_TIME_MS 500

// Button states
static button_state_t button_states[4];
static uint8_t button_pins[4] = {BTN_A, BTN_B, BTN_X, BTN_Y};

// Previous frame button states for edge detection
static bool prev_button_state[4];

// Doom key states
static bool doom_key_states[DOOM_KEY_COUNT];
static bool prev_doom_key_states[DOOM_KEY_COUNT];

// Debug string buffer
static char debug_buffer[128];

/**
 * Initialize input system
 */
void input_init(void) {
    printf("Initializing input handler...\n");
    
    // Initialize all buttons
    for (int i = 0; i < 4; i++) {
        gpio_init(button_pins[i]);
        gpio_set_dir(button_pins[i], GPIO_IN);
        gpio_pull_up(button_pins[i]);  // Buttons are active low
        
        button_states[i].pressed = false;
        button_states[i].held = false;
        button_states[i].press_time = 0;
        button_states[i].release_time = 0;
        
        prev_button_state[i] = false;
    }
    
    // Initialize doom key states
    memset(doom_key_states, 0, sizeof(doom_key_states));
    memset(prev_doom_key_states, 0, sizeof(prev_doom_key_states));
    
    printf("Input handler initialized\n");
    printf("Controls:\n");
    printf("  A: Fire/Use\n");
    printf("  B: Open/Activate\n");
    printf("  X: Next weapon\n");
    printf("  Y: Previous weapon\n");
    printf("  A+B: Move forward\n");
    printf("  A+Y: Move backward\n");
    printf("  B+X: Strafe left\n");
    printf("  B+Y: Strafe right\n");
    printf("  Hold X: Turn left\n");
    printf("  Hold Y: Turn right\n");
    printf("  X+Y: Toggle map\n");
}

/**
 * Read raw button state (active low)
 */
static inline bool read_button(uint8_t button_index) {
    return !gpio_get(button_pins[button_index]);  // Inverted (active low)
}

/**
 * Update button states with debouncing
 */
static void update_button_states(void) {
    uint32_t now = to_ms_since_boot(get_absolute_time());
    
    for (int i = 0; i < 4; i++) {
        bool current = read_button(i);
        bool previous = prev_button_state[i];
        
        // Edge detection with debouncing
        if (current && !previous) {
            // Button just pressed
            if (now - button_states[i].release_time > DEBOUNCE_MS) {
                button_states[i].pressed = true;
                button_states[i].press_time = now;
                button_states[i].held = false;
            }
        } else if (!current && previous) {
            // Button just released
            if (now - button_states[i].press_time > DEBOUNCE_MS) {
                button_states[i].pressed = false;
                button_states[i].release_time = now;
                button_states[i].held = false;
            }
        } else if (current && previous) {
            // Button held
            if (now - button_states[i].press_time > HOLD_TIME_MS) {
                button_states[i].held = true;
            }
        }
        
        prev_button_state[i] = current;
    }
}

/**
 * Map button states to Doom keys
 */
static void map_doom_keys(void) {
    // Save previous state
    memcpy(prev_doom_key_states, doom_key_states, sizeof(doom_key_states));
    memset(doom_key_states, 0, sizeof(doom_key_states));
    
    bool btn_a = button_states[0].pressed;
    bool btn_b = button_states[1].pressed;
    bool btn_x = button_states[2].pressed;
    bool btn_y = button_states[3].pressed;
    
    bool hold_x = button_states[2].held;
    bool hold_y = button_states[3].held;
    
    // Check for button combinations first (priority)
    if (btn_a && btn_b && btn_x && btn_y) {
        // All buttons = menu
        doom_key_states[DOOM_KEY_MENU] = true;
    } else if (btn_x && btn_y) {
        // X + Y = Map
        doom_key_states[DOOM_KEY_MAP] = true;
    } else if (btn_a && btn_b) {
        // A + B = Forward
        doom_key_states[DOOM_KEY_FORWARD] = true;
    } else if (btn_a && btn_y) {
        // A + Y = Backward
        doom_key_states[DOOM_KEY_BACKWARD] = true;
    } else if (btn_b && btn_x) {
        // B + X = Strafe Left
        doom_key_states[DOOM_KEY_STRAFE_LEFT] = true;
    } else if (btn_b && btn_y) {
        // B + Y = Strafe Right
        doom_key_states[DOOM_KEY_STRAFE_RIGHT] = true;
    } else {
        // Single button actions
        if (btn_a) {
            doom_key_states[DOOM_KEY_FIRE] = true;
        }
        if (btn_b) {
            doom_key_states[DOOM_KEY_USE] = true;
        }
        if (btn_x && !hold_x) {
            doom_key_states[DOOM_KEY_WEAPON_UP] = true;
        }
        if (btn_y && !hold_y) {
            doom_key_states[DOOM_KEY_WEAPON_DOWN] = true;
        }
        if (hold_x) {
            doom_key_states[DOOM_KEY_TURN_LEFT] = true;
        }
        if (hold_y) {
            doom_key_states[DOOM_KEY_TURN_RIGHT] = true;
        }
    }
}

/**
 * Update input state
 */
void input_update(void) {
    update_button_states();
    map_doom_keys();
}

/**
 * Check if a Doom key is currently pressed
 */
bool input_is_key_down(doom_key_t key) {
    if (key >= DOOM_KEY_COUNT) return false;
    return doom_key_states[key];
}

/**
 * Check if a Doom key was just pressed this frame
 */
bool input_is_key_pressed(doom_key_t key) {
    if (key >= DOOM_KEY_COUNT) return false;
    return doom_key_states[key] && !prev_doom_key_states[key];
}

/**
 * Check if a Doom key was just released this frame
 */
bool input_is_key_released(doom_key_t key) {
    if (key >= DOOM_KEY_COUNT) return false;
    return !doom_key_states[key] && prev_doom_key_states[key];
}

/**
 * Get raw button state
 */
bool input_is_button_down(uint8_t button) {
    for (int i = 0; i < 4; i++) {
        if (button_pins[i] == button) {
            return button_states[i].pressed;
        }
    }
    return false;
}

/**
 * Check for button combinations
 */
bool input_is_combo(uint8_t btn1, uint8_t btn2) {
    return input_is_button_down(btn1) && input_is_button_down(btn2);
}

/**
 * Get debug info string
 */
const char* input_get_debug_string(void) {
    snprintf(debug_buffer, sizeof(debug_buffer),
             "Buttons: A=%d B=%d X=%d Y=%d | Keys: FWD=%d FIRE=%d USE=%d",
             button_states[0].pressed,
             button_states[1].pressed,
             button_states[2].pressed,
             button_states[3].pressed,
             doom_key_states[DOOM_KEY_FORWARD],
             doom_key_states[DOOM_KEY_FIRE],
             doom_key_states[DOOM_KEY_USE]);
    return debug_buffer;
}
