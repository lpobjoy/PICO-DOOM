# PICO-DOOM Releases

This directory contains compiled firmware releases ready to flash to your Raspberry Pi Pico.

## Latest Release

### Phase 3: Rendering Engine & WAD Loader (v0.3.0)
**Date**: November 17, 2025  
**File**: `pico_doom_phase3.uf2` (87 KB)  
**Status**: ✅ Production-ready for hardware testing

**Features**:
- ✅ Display rendering (320×240 ST7789)
- ✅ 3 interactive test patterns:
  - Color bars (display calibration)
  - Checkerboard (precision testing)
  - Gradient (frame rate validation)
- ✅ Button input with debouncing
- ✅ WAD file loader framework
- ✅ USB serial debugging

**Hardware Requirements**:
- Raspberry Pi Pico
- Pimoroni Pico Display 2.0 (320×240)
- USB cable for power/debugging

**How to Flash**:

```bash
# Method 1: Bootloader Mode (Easiest)
1. Plug in Pico while holding BOOTSEL button
2. Drag pico_doom_phase3.uf2 to PICO-RP2 drive
3. Pico reboots automatically

# Method 2: Using picotool
picotool load pico_doom_phase3.uf2 -v
```

**Testing the Firmware**:

1. Open USB serial terminal (baud 115200)
2. Press X button to cycle through patterns
3. Press Y button to go to previous pattern
4. Press A/B buttons to verify input detection

---

## Version History

### Phase 2: Doom Engine Wrapper (v0.2.0)
- Doom engine abstraction layer
- Input integration framework
- Dual-core rendering pipeline
- Status: ✅ Compile verified

### Phase 1: Build System & Hardware Init (v0.1.0)
- Project setup and build system
- Display driver integration
- Input handler implementation
- Status: ✅ Hardware verified

---

## Build Information

| Component | Details |
|-----------|---------|
| **Compiler** | ARM GCC 14.3.1 |
| **Pico SDK** | 1.5.0+ |
| **Binary Size** | 87 KB UF2 (741 KB ELF) |
| **RAM Usage** | ~200 KB of 264 KB available |
| **Target FPS** | 30+ FPS minimum |
| **Compilation** | Zero errors/warnings |

---

## Changelog

### Phase 3 (Current)
- Implemented RGB565 rendering with ST7789 byte-swap
- Added 3 test patterns for hardware validation
- Created WAD file loader with IWAD/PWAD detection
- Enhanced .gitignore with security best practices
- Added comprehensive documentation

### Phase 2
- Created doom_engine abstraction layer
- Integrated input handling into game loop
- Established dual-core rendering architecture

### Phase 1
- Configured CMake build system
- Integrated Pimoroni display driver
- Implemented button input handler

---

## Support & Next Steps

### Phase 4: Hardware Testing & Optimization
- Validate display on real hardware
- Profile frame rates and latency
- Optimize rendering performance
- Test all input combinations

### Phase 5: Full Doom Engine Integration
- Load DOOM1.WAD file
- Integrate PicoDoom rendering
- Implement player movement
- Add sprites and gameplay mechanics

### Documentation
- See `docs/BUILDING.md` for build instructions
- See `docs/PHASE3_RENDERING.md` for Phase 3 details
- See `QUICKSTART.md` for quick start guide

---

## Firmware File Format

All releases use the **UF2 (USB Flashing Format)**, specifically targeting:
- **Family**: Raspberry Pi RP2040
- **Start Address**: 0x10000000 (flash memory)
- **Total Blocks**: 173 (Phase 3)

UF2 format allows drag-and-drop flashing without additional tools.

---

*Last updated: November 17, 2025*
