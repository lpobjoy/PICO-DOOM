/**
 * Input handler for PICO-DOOM
 * Maps Pimoroni display buttons to Doom controls
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

// Button definitions (Pimoroni Pico Display 2.0)
#define BTN_A  12
#define BTN_B  13
#define BTN_X  14
#define BTN_Y  15

// Doom key mappings
typedef enum {
    DOOM_KEY_FIRE = 0,     // A button
    DOOM_KEY_USE,          // B button
    DOOM_KEY_WEAPON_UP,    // X button
    DOOM_KEY_WEAPON_DOWN,  // Y button (hold)
    DOOM_KEY_FORWARD,      // A + B
    DOOM_KEY_BACKWARD,     // A + Y
    DOOM_KEY_STRAFE_LEFT,  // B + X
    DOOM_KEY_STRAFE_RIGHT, // B + Y
    DOOM_KEY_TURN_LEFT,    // X (hold)
    DOOM_KEY_TURN_RIGHT,   // Y (tap)
    DOOM_KEY_MAP,          // X + Y
    DOOM_KEY_MENU,         // Hold all buttons
    DOOM_KEY_COUNT
} doom_key_t;

// Button state
typedef struct {
    bool pressed;
    bool held;
    uint32_t press_time;
    uint32_t release_time;
} button_state_t;

/**
 * Initialize input system
 */
void input_init(void);

/**
 * Update input state (call every frame)
 */
void input_update(void);

/**
 * Check if a Doom key is currently pressed
 */
bool input_is_key_down(doom_key_t key);

/**
 * Check if a Doom key was just pressed this frame
 */
bool input_is_key_pressed(doom_key_t key);

/**
 * Check if a Doom key was just released this frame
 */
bool input_is_key_released(doom_key_t key);

/**
 * Get raw button state
 */
bool input_is_button_down(uint8_t button);

/**
 * Check for button combinations
 */
bool input_is_combo(uint8_t btn1, uint8_t btn2);

/**
 * Get debug info string
 */
const char* input_get_debug_string(void);

#endif // INPUT_HANDLER_H
