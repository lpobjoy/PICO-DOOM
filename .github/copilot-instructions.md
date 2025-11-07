# PICO-DOOM Project Instructions

## Project Overview
This project ports Doom to the Raspberry Pi Pico using the Pimoroni LCD GFX libraries. We're building on the PicoDoom project, which is optimized for the RP2040 microcontroller's constraints (264KB RAM, dual-core 133MHz ARM Cortex-M0+).

## Hardware Target
- **Microcontroller**: Raspberry Pi Pico (RP2040)
- **Display**: Pimoroni LCD display (ST7789 driver, SPI interface)
  - Pico Display Pack (240x135) or
  - Pico Display 2.0 (320x240)
- **Input**: Pimoroni display pack buttons (A, B, X, Y)
- **Audio**: PWM audio output (optional)

## Technical Architecture

### Core Components
1. **Doom Engine**: Simplified Doom engine optimized for embedded systems
2. **Graphics Backend**: Pimoroni Pico Graphics library integration
3. **Input Handler**: Button mapping for Doom controls
4. **Memory Management**: Optimized for 264KB RAM constraint
5. **WAD Loader**: Support for shareware Doom1 WAD file

### Development Stack
- **Language**: C/C++
- **SDK**: Raspberry Pi Pico SDK (pico-sdk)
- **Graphics Library**: Pimoroni Pico Graphics HAL
- **Build System**: CMake
- **Toolchain**: ARM GCC cross-compiler

## Code Standards

### File Organization
```
PICO-DOOM/
├── src/              # Source code
│   ├── doom/         # Doom engine code
│   ├── pico/         # Pico-specific implementations
│   ├── display/      # Display driver integration
│   └── input/        # Input handling
├── include/          # Header files
├── lib/              # Third-party libraries
├── wad/              # WAD files (not committed)
├── CMakeLists.txt    # Build configuration
└── README.md         # Project documentation
```

### Naming Conventions
- **Files**: lowercase_with_underscores.c/h
- **Functions**: camelCase for new code, keep original Doom naming where possible
- **Macros**: UPPER_CASE_WITH_UNDERSCORES
- **Types**: PascalCase with _t suffix for embedded types

### Memory Optimization Rules
1. Use static allocation where possible (avoid malloc/free in game loop)
2. Keep stack usage minimal (< 4KB per core)
3. Store large assets (textures, sprites) in flash, not RAM
4. Use 8-bit and 16-bit types aggressively
5. Optimize WAD loading to minimize RAM footprint

### Display Integration
- Target 30 FPS minimum, 60 FPS ideal
- Use double buffering to prevent tearing
- Scale down resolution if needed for performance
- Leverage both RP2040 cores (rendering on Core 1)

### Input Mapping
- **A Button**: Fire/Use
- **B Button**: Open/Activate
- **X Button**: Weapon change
- **Y Button**: Map
- **D-Pad/Arrows**: Movement and turning

## Performance Requirements
- Minimum 20 FPS gameplay
- Sub-100ms input latency
- Smooth rendering without visible tearing
- Stable frame times (no stuttering)

## Build Instructions Context
When generating build files or instructions:
1. Use Pico SDK 1.5.0 or later
2. Include Pimoroni libraries via git submodule or FetchContent
3. Enable USB output for debugging
4. Support both Debug and Release configurations
5. Generate .uf2 files for easy flashing

## Testing Guidelines
- Test on actual hardware, not just QEMU
- Profile memory usage regularly
- Monitor frame rates during gameplay
- Test all input combinations
- Verify display rendering at all orientations

## Known Constraints
- **RAM**: 264KB total (shared between both cores)
- **Flash**: 2MB (enough for code + small WAD)
- **CPU**: 133MHz (can overclock to ~250MHz if needed)
- **Display**: SPI limited to ~60-80 FPS theoretical max
- **No FPU**: Avoid floating-point math where possible

## Doom-Specific Implementation Notes
- Use shareware Doom1 WAD (DOOM1.WAD) - smaller and freely distributable
- Reduce texture resolution to save RAM
- Simplify enemy AI if needed for performance
- Consider reducing level geometry complexity
- Audio is optional - prioritize graphics and gameplay first

## External Resources
- [PicoDoom GitHub Repository](https://github.com/kilograham/rp2040-doom)
- [Pico SDK Documentation](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Pimoroni Pico Graphics](https://github.com/pimoroni/pimoroni-pico)
- [Doom Wiki](https://doomwiki.org/)

## Development Priorities
1. **Phase 1**: Get PicoDoom compiling with Pico SDK
2. **Phase 2**: Integrate Pimoroni display driver
3. **Phase 3**: Implement input handling
4. **Phase 4**: Optimize performance and memory
5. **Phase 5**: Polish and audio (if time permits)

## Git Workflow
- Main branch: stable, working builds only
- Feature branches: individual features/fixes
- Commit messages: Clear, descriptive (e.g., "Add ST7789 display driver integration")
- Don't commit WAD files (add to .gitignore)
- Do commit build instructions and documentation

## When Making Changes
- Always consider memory impact
- Test on hardware after significant changes
- Document any performance optimizations
- Keep code readable - embedded doesn't mean cryptic
- Comment any non-obvious RP2040-specific workarounds
