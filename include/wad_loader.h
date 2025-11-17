/**
 * WAD file loader for PICO-DOOM
 * Handles loading and parsing Doom WAD files
 */

#ifndef WAD_LOADER_H
#define WAD_LOADER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * WAD file header
 */
typedef struct {
    char identification[4];  // "IWAD" or "PWAD"
    uint32_t numlumps;       // Number of lumps in WAD
    uint32_t infotableofs;   // Offset to lump directory
} wad_header_t;

/**
 * WAD lump directory entry
 */
typedef struct {
    uint32_t filepos;        // Position in WAD file
    uint32_t size;           // Size of lump
    char name[8];            // Lump name
} wad_lump_t;

/**
 * WAD file structure
 */
typedef struct {
    wad_header_t header;
    wad_lump_t *lumps;       // Directory of lumps
    bool is_iwad;            // true for IWAD, false for PWAD
    uint32_t num_lumps;      // Cached lump count
    const uint8_t *data;     // Pointer to WAD data in memory
    uint32_t data_size;      // Size of loaded data
} wad_file_t;

/**
 * Load a WAD file from memory
 * Returns pointer to WAD structure on success, NULL on failure
 */
wad_file_t* wad_load_from_memory(const uint8_t *data, uint32_t size);

/**
 * Find a lump by name
 * Returns lump entry if found, NULL otherwise
 */
wad_lump_t* wad_find_lump(wad_file_t *wad, const char *name);

/**
 * Get lump data pointer
 * Returns pointer to lump data in the WAD
 */
const uint8_t* wad_get_lump_data(wad_file_t *wad, wad_lump_t *lump);

/**
 * Free WAD structure
 */
void wad_free(wad_file_t *wad);

/**
 * Print WAD file information (debugging)
 */
void wad_print_info(wad_file_t *wad);

#ifdef __cplusplus
}
#endif

#endif // WAD_LOADER_H
