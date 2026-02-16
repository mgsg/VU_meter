cmake_minimum_required(VERSION 3.12)

# Default SDK path to submodule location if not set via environment
if (NOT PICO_SDK_PATH AND NOT DEFINED ENV{PICO_SDK_PATH})
    set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/VU_meter/pico-sdk)
endif()

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

# Set the project name and specify the target
project(vu_meter C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

add_subdirectory(src)