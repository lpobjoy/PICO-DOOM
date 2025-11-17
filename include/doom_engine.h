/**
 * Doom engine wrapper for PICO-DOOM
 * Provides a simplified interface to the Doom engine
 */

#ifndef DOOM_ENGINE_H
#define DOOM_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Doom key states
 */
typedef struct {
    bool forward;
    bool backward;
    bool strafe_left;
    bool strafe_right;
    bool turn_left;
    bool turn_right;
    bool fire;
    bool use;
    bool weapon_next;
    bool weapon_prev;
} doom_input_t;

/**
 * Initialize Doom engine
 * Returns true on success, false on failure
 */
bool doom_init(void);

/**
 * Load a WAD file
 * Returns true on success
 */
bool doom_load_wad(const char *wad_path);

/**
 * Update Doom engine with input and advance one game tick
 */
void doom_update(const doom_input_t *input);

/**
 * Render current frame to framebuffer
 * Framebuffer should be DISPLAY_WIDTH x DISPLAY_HEIGHT pixels
 */
void doom_render(uint16_t *framebuffer);

/**
 * Get current game state info
 */
void doom_get_state(char *buffer, int max_len);

/**
 * Cleanup and shutdown
 */
void doom_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif // DOOM_ENGINE_H
