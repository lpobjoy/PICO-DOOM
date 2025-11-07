# Building PICO-DOOM

## Prerequisites

### Required Software

1. **Raspberry Pi Pico SDK**
   - Download from: https://github.com/raspberrypi/pico-sdk
   - Recommended version: 1.5.0 or later

2. **ARM GCC Toolchain**
   - **macOS**: `brew install gcc-arm-embedded`
   - **Linux**: `sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib`
   - **Windows**: Download from ARM website

3. **CMake** (3.13+)
   - **macOS**: `brew install cmake`
   - **Linux**: `sudo apt install cmake`
   - **Windows**: Download from cmake.org

4. **Build Tools**
   - **macOS**: Xcode Command Line Tools (`xcode-select --install`)
   - **Linux**: `sudo apt install build-essential`
   - **Windows**: Visual Studio Build Tools

## Setup Instructions

### 1. Install Pico SDK

```bash
# Clone the Pico SDK
cd ~/
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

# Set environment variable (add to ~/.zshrc or ~/.bashrc)
export PICO_SDK_PATH=~/pico-sdk
```

### 2. Clone PICO-DOOM

```bash
git clone <repository-url>
cd PICO-DOOM

# Initialize submodules (including Pimoroni libraries)
git submodule update --init --recursive
```

### 3. Add Pimoroni Libraries

```bash
# Add Pimoroni Pico libraries as submodule
git submodule add https://github.com/pimoroni/pimoroni-pico.git lib/pimoroni-pico
git submodule update --init --recursive
```

### 4. Add PicoDoom Source

```bash
# Add PicoDoom as submodule or copy source files
git submodule add https://github.com/kilograham/rp2040-doom.git lib/picodoom
```

## Building the Project

### Standard Build

```bash
# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build
make -j4

# Output will be: pico_doom.uf2
```

### Release Build (Optimized)

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

### Debug Build

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4
```

## Flashing to Pico

1. Hold the BOOTSEL button on your Pico
2. Connect it to your computer via USB
3. Release BOOTSEL - Pico will appear as USB drive
4. Copy `pico_doom.uf2` to the Pico drive
5. Pico will automatically reboot and run Doom!

## Debugging

### USB Serial Output

The project is configured to output debug messages over USB serial:

```bash
# macOS/Linux
screen /dev/tty.usbmodem* 115200

# Or use minicom
minicom -D /dev/tty.usbmodem* -b 115200

# Windows
# Use PuTTY or other serial terminal
```

### Common Build Issues

**Error: PICO_SDK_PATH not set**
```bash
export PICO_SDK_PATH=~/pico-sdk
```

**Error: ARM toolchain not found**
- Ensure `arm-none-eabi-gcc` is in your PATH
- Verify installation: `arm-none-eabi-gcc --version`

**Error: Pimoroni libraries not found**
```bash
git submodule update --init --recursive
```

## Optimizations

### Overclocking (Optional)

To run at higher speed, modify `src/main.c`:

```c
// Set to 250 MHz (from default 133 MHz)
set_sys_clock_khz(250000, true);
```

**Warning**: May cause instability. Test thoroughly!

### Memory Profiling

Check memory usage after building:

```bash
arm-none-eabi-size build/pico_doom.elf
```

Target: < 264KB RAM usage

## Next Steps

- Add WAD file (see [WAD_SETUP.md](WAD_SETUP.md))
- Configure display settings
- Test on hardware
- Profile performance

## Troubleshooting

See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for common issues and solutions.
