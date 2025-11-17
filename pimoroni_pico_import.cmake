# This file can be dropped into a project to help locate the Pimoroni Pico libraries
# It will also set up the required include and module search paths.

if (NOT PIMORONI_PICO_PATH)
    set(PIMORONI_PICO_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/pimoroni-pico)
endif()

if (NOT EXISTS ${PIMORONI_PICO_PATH})
    message(WARNING "Pimoroni Pico libraries not found at ${PIMORONI_PICO_PATH}")
    message(WARNING "Run: git submodule update --init --recursive")
else()
    message(STATUS "Pimoroni Pico libraries found at ${PIMORONI_PICO_PATH}")
    
    # Add to the module path
    list(APPEND CMAKE_MODULE_PATH ${PIMORONI_PICO_PATH})
    
    # Include the Pimoroni libraries
    include(${PIMORONI_PICO_PATH}/pimoroni_pico_import.cmake)
endif()
