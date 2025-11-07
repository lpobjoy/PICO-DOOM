#!/bin/bash

# PICO-DOOM Setup Script
# This script helps set up the development environment for building PICO-DOOM

set -e

echo "================================"
echo "   PICO-DOOM Setup Script"
echo "================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if Pico SDK is installed
echo "Checking for Pico SDK..."
if [ -z "$PICO_SDK_PATH" ]; then
    echo -e "${RED}✗ PICO_SDK_PATH not set${NC}"
    echo ""
    echo "Do you want to install Pico SDK now? (y/n)"
    read -r response
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
        PICO_SDK_INSTALL_DIR="$HOME/pico-sdk"
        echo "Installing Pico SDK to $PICO_SDK_INSTALL_DIR..."
        
        if [ -d "$PICO_SDK_INSTALL_DIR" ]; then
            echo -e "${YELLOW}⚠ Pico SDK directory already exists. Updating...${NC}"
            cd "$PICO_SDK_INSTALL_DIR"
            git pull
        else
            git clone https://github.com/raspberrypi/pico-sdk.git "$PICO_SDK_INSTALL_DIR"
            cd "$PICO_SDK_INSTALL_DIR"
        fi
        
        git submodule update --init
        
        export PICO_SDK_PATH="$PICO_SDK_INSTALL_DIR"
        
        echo ""
        echo -e "${GREEN}✓ Pico SDK installed!${NC}"
        echo ""
        echo "Add this to your shell profile (~/.zshrc or ~/.bashrc):"
        echo "export PICO_SDK_PATH=$PICO_SDK_INSTALL_DIR"
        echo ""
    else
        echo "Please install Pico SDK manually and set PICO_SDK_PATH"
        exit 1
    fi
else
    echo -e "${GREEN}✓ PICO_SDK_PATH found: $PICO_SDK_PATH${NC}"
fi

# Check for ARM toolchain
echo ""
echo "Checking for ARM GCC toolchain..."
if command -v arm-none-eabi-gcc &> /dev/null; then
    VERSION=$(arm-none-eabi-gcc --version | head -n1)
    echo -e "${GREEN}✓ ARM toolchain found: $VERSION${NC}"
else
    echo -e "${RED}✗ ARM toolchain not found${NC}"
    echo ""
    
    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "Install with: brew install gcc-arm-embedded"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "Install with: sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib"
    fi
    
    echo ""
    echo "Install ARM toolchain and run this script again."
    exit 1
fi

# Check for CMake
echo ""
echo "Checking for CMake..."
if command -v cmake &> /dev/null; then
    VERSION=$(cmake --version | head -n1)
    echo -e "${GREEN}✓ CMake found: $VERSION${NC}"
else
    echo -e "${RED}✗ CMake not found${NC}"
    
    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "Install with: brew install cmake"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "Install with: sudo apt install cmake"
    fi
    
    exit 1
fi

# Initialize git submodules
echo ""
echo "Initializing git submodules..."
cd "$(dirname "$0")"
git submodule update --init --recursive
echo -e "${GREEN}✓ Submodules initialized${NC}"

# Create build directory
echo ""
echo "Setting up build directory..."
if [ ! -d "build" ]; then
    mkdir build
    echo -e "${GREEN}✓ Build directory created${NC}"
else
    echo -e "${YELLOW}⚠ Build directory already exists${NC}"
fi

echo ""
echo "================================"
echo -e "${GREEN}Setup complete!${NC}"
echo "================================"
echo ""
echo "Next steps:"
echo "1. Add WAD file to wad/ directory (see docs/WAD_SETUP.md)"
echo "2. Build: cd build && cmake .. && make"
echo "3. Flash: Copy build/pico_doom.uf2 to Pico"
echo ""
echo "For detailed instructions, see docs/BUILDING.md"
echo ""
