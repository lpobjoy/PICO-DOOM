# Quick Start Guide - PICO-DOOM

## Summary

**PICO-DOOM Phases 1-3 are complete and ready for hardware testing.** The firmware includes interactive test patterns and a complete WAD file loader framework.

## What You Have

### Firmware
- **File**: `build/pico_doom.uf2` (87 KB)
- **Status**: ✅ Ready to flash to Pico
- **Compilation**: Clean (no errors/warnings)

### Features
- ✅ 320×240 display rendering (Pimoroni ST7789)
- ✅ 3 interactive test patterns (color bars, checkerboard, gradient)
- ✅ Button input with debouncing
- ✅ WAD file loader framework
- ✅ USB serial debugging
- ✅ Dual-core rendering pipeline

## Quick Start Steps

### 1. Flash Firmware to Pico

```bash
# Option A: Bootloader Mode (Easiest)
1. Plug in Pico while holding BOOTSEL button
2. Drag build/pico_doom.uf2 to PICO-RP2 drive
3. Pico reboots automatically

# Option B: Using picotool
picotool load build/pico_doom.uf2
```

### 2. Connect USB Serial Console

Open serial terminal to see debug output:
```bash
# macOS/Linux
screen /dev/tty.usbmodem14101 115200

# Or use another terminal app (baud: 115200)
```

You should see:
```
PICO-DOOM Initializing...
RP2040 running at 133 MHz
...
Display ready: 320x240
```

### 3. Test Display Patterns

The display should show **Color Bars** pattern (6 vertical stripes).

**Control patterns:**
- Press **X button** (Weapon_Up) → Next pattern
- Press **Y button** (Weapon_Down) → Previous pattern

**Patterns:**
1. **Color Bars** - 6 colored vertical stripes
2. **Checkerboard** - Black & white 8×8 checkers
3. **Gradient** - Animated RGB gradient

### 4. Verify Input

Press buttons and watch USB output for:
```
Button pressed: A
Button pressed: B
Button pressed: X
Button pressed: Y
```

## File Organization

```
build/
  └─ pico_doom.uf2           ← FLASH THIS FILE

src/
  ├─ main.c                  (Game loop)
  ├─ doom_engine.c           (Test patterns + rendering)
  ├─ wad_loader.c            (WAD file handling)
  ├─ display/
  └─ input/

include/
  ├─ doom_engine.h
  ├─ wad_loader.h
  ├─ display_adapter.h
  └─ input_handler.h

docs/
  ├─ PHASE3_RENDERING.md     (Phase 3 details)
  ├─ BUILD_PROGRESS.md       (Project status)
  └─ BUILDING.md             (Build instructions)
```

## Rebuild Firmware

```bash
cd /Users/Lewis.Pobjoy/Documents/Personal/PICO-DOOM/build
make
# Output: [100%] Built target pico_doom ✅
```

## Hardware Requirements

- **Pico**: Raspberry Pi Pico
- **Display**: Pimoroni Pico Display 2.0 (320×240 ST7789)
- **Power**: USB cable

## Troubleshooting

### Display shows nothing
- Check USB serial output for errors
- Verify display is connected properly
- Try different test pattern (press X button)

### Buttons not responding
- Check USB output for button presses
- Verify button pins are correct (A=12, B=13, X=14, Y=15)

### High memory usage
- Normal - firmware uses ~200KB of 264KB available
- No immediate optimizations needed

## What's Next

### Phase 4: Hardware Testing & Optimization
- Validate display performance on real hardware
- Profile frame rates and latency
- Optimize rendering if needed

### Phase 5: Full Doom Engine
- Load DOOM1.WAD file
- Integrate PicoDoom rendering
- Implement player movement
- Add sprites and gameplay

## Documentation

| Document | Content |
|----------|---------|
| `README.md` | Main project overview |
| `docs/PHASE3_RENDERING.md` | Phase 3 implementation details |
| `docs/BUILD_PROGRESS.md` | Overall project status |
| `docs/BUILDING.md` | Detailed build instructions |
| `.github/copilot-instructions.md` | Development guidelines |

## Support

For issues or questions:
1. Check `docs/BUILDING.md` for build issues
2. Review `docs/PHASE3_RENDERING.md` for rendering questions
3. Check USB serial output for debug messages

---

**Status: Ready for hardware testing!** 🎮

*Last updated: November 17, 2025*
