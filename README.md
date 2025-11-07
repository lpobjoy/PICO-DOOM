# PICO-DOOM

Run classic Doom on a Raspberry Pi Pico with Pimoroni LCD display!

## 🎮 Project Status

This project ports Doom to the RP2040 microcontroller using the PicoDoom engine and Pimoroni's graphics libraries.

**Current Status**: 🚧 In Development

## 🔧 Hardware Requirements

- Raspberry Pi Pico (RP2040)
- Pimoroni Display Pack or Pico Display 2.0
  - Pico Display Pack: 240x135 pixels (ST7789)
  - Pico Display 2.0: 320x240 pixels (ST7789)
- USB cable for power and programming
- (Optional) Speaker/buzzer for audio

## 🛠️ Software Requirements

- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) (v1.5.0+)
- ARM GCC cross-compiler
- CMake (3.13+)
- Python 3.x
- Git

## 📦 Building

Detailed build instructions coming soon!

```bash
# Clone the repository
git clone <repository-url>
cd PICO-DOOM

# Initialize submodules
git submodule update --init --recursive

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Flash to Pico (drag .uf2 file to PICO drive)
```

## 🎯 Controls

- **A Button**: Fire weapon / Use door
- **B Button**: Open / Activate
- **X Button**: Change weapon
- **Y Button**: Toggle map
- **D-Pad**: Movement and turning

## 📋 Features

- [x] Project planning and architecture
- [ ] Basic Doom engine integration
- [ ] Pimoroni display driver
- [ ] Input handling
- [ ] Performance optimization
- [ ] Audio support (optional)

## 🙏 Credits

- **PicoDoom**: Based on the excellent work by Graham Sanderson
- **id Software**: Original Doom game
- **Pimoroni**: Display libraries and hardware
- **Raspberry Pi Foundation**: Pico SDK

## 📄 License

This project respects the licenses of all components:
- Doom source code: GPL-2.0
- Pico SDK: BSD-3-Clause
- Pimoroni libraries: MIT

See individual component licenses for details.

## 🚀 Getting Started

Check out our [Copilot Instructions](.github/copilot-instructions.md) for detailed development guidelines.

---

**Made with ❤️ for the RP2040 community**
