cmake_minimum_required(VERSION 3.13)

# Pull in Pico SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in Pimoroni libraries
include(pimoroni_pico_import.cmake)

project(pico_doom C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src)

# Main executable
add_executable(pico_doom
    src/main.c
)

# Pull in common dependencies
target_link_libraries(pico_doom
    pico_stdlib
    pico_multicore
    hardware_spi
    hardware_pwm
    hardware_dma
)

# Enable USB output for debugging, disable UART
pico_enable_stdio_usb(pico_doom 1)
pico_enable_stdio_uart(pico_doom 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(pico_doom)

# Add compile options
target_compile_options(pico_doom PRIVATE
    -Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

# Optimization flags for Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(pico_doom PRIVATE -O3)
endif()

# Add include directories
target_include_directories(pico_doom PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Print build info
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Pico SDK path: $ENV{PICO_SDK_PATH}")
