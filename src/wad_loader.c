/**
 * WAD file loader implementation
 * Minimal implementation for Doom WAD file support
 */

#include "wad_loader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

wad_file_t* wad_load_from_memory(const uint8_t *data, uint32_t size) {
    if (!data || size < sizeof(wad_header_t)) {
        printf("Error: Invalid WAD data\n");
        return NULL;
    }
    
    // Read header
    const wad_header_t *header = (const wad_header_t *)data;
    
    // Validate header
    if (memcmp(header->identification, "IWAD", 4) != 0 && 
        memcmp(header->identification, "PWAD", 4) != 0) {
        printf("Error: Not a valid WAD file (bad magic)\n");
        return NULL;
    }
    
    // Allocate WAD structure
    wad_file_t *wad = (wad_file_t *)malloc(sizeof(wad_file_t));
    if (!wad) {
        printf("Error: Failed to allocate WAD structure\n");
        return NULL;
    }
    
    // Copy header
    memcpy(&wad->header, header, sizeof(wad_header_t));
    wad->is_iwad = (memcmp(header->identification, "IWAD", 4) == 0);
    wad->data = data;
    wad->data_size = size;
    wad->num_lumps = header->numlumps;
    
    // Load lump directory
    uint32_t lump_table_offset = header->infotableofs;
    if (lump_table_offset + (header->numlumps * sizeof(wad_lump_t)) > size) {
        printf("Error: Lump table extends beyond WAD size\n");
        free(wad);
        return NULL;
    }
    
    const wad_lump_t *lump_table = (const wad_lump_t *)(data + lump_table_offset);
    
    // Allocate and copy lump directory
    wad->lumps = (wad_lump_t *)malloc(header->numlumps * sizeof(wad_lump_t));
    if (!wad->lumps) {
        printf("Error: Failed to allocate lump directory\n");
        free(wad);
        return NULL;
    }
    
    memcpy(wad->lumps, lump_table, header->numlumps * sizeof(wad_lump_t));
    
    printf("WAD loaded successfully: %s\n", wad->is_iwad ? "IWAD" : "PWAD");
    printf("  Lumps: %d\n", header->numlumps);
    printf("  Size: %d bytes\n", size);
    
    return wad;
}

wad_lump_t* wad_find_lump(wad_file_t *wad, const char *name) {
    if (!wad || !name) {
        return NULL;
    }
    
    for (uint32_t i = 0; i < wad->num_lumps; i++) {
        // WAD lump names are 8-char, space-padded
        if (strncmp(wad->lumps[i].name, name, 8) == 0) {
            return &wad->lumps[i];
        }
    }
    
    return NULL;
}

const uint8_t* wad_get_lump_data(wad_file_t *wad, wad_lump_t *lump) {
    if (!wad || !lump) {
        return NULL;
    }
    
    // Validate lump is within WAD bounds
    if (lump->filepos + lump->size > wad->data_size) {
        printf("Error: Lump extends beyond WAD data\n");
        return NULL;
    }
    
    return wad->data + lump->filepos;
}

void wad_free(wad_file_t *wad) {
    if (!wad) {
        return;
    }
    
    if (wad->lumps) {
        free(wad->lumps);
    }
    
    free(wad);
}

void wad_print_info(wad_file_t *wad) {
    if (!wad) {
        return;
    }
    
    printf("WAD Information:\n");
    printf("  Type: %s\n", wad->is_iwad ? "IWAD" : "PWAD");
    printf("  Lumps: %d\n", wad->num_lumps);
    printf("  Total size: %d bytes\n", wad->data_size);
    printf("\nFirst 20 lumps:\n");
    
    int count = (wad->num_lumps < 20) ? wad->num_lumps : 20;
    for (int i = 0; i < count; i++) {
        char name[9];
        memcpy(name, wad->lumps[i].name, 8);
        name[8] = '\0';
        printf("  [%3d] %8s - %8d bytes @ 0x%08X\n", 
               i, name, wad->lumps[i].size, wad->lumps[i].filepos);
    }
    
    if (wad->num_lumps > 20) {
        printf("  ... and %d more lumps\n", wad->num_lumps - 20);
    }
}
