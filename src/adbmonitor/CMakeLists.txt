# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/akuker/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(adbmonitor C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(adbmonitor adbmonitor.cpp adb.cpp)

pico_set_program_name(adbmonitor "adbmonitor")
pico_set_program_version(adbmonitor "0.1")

pico_enable_stdio_uart(adbmonitor 1)
pico_enable_stdio_usb(adbmonitor 0)

# Add the standard library to the build
target_link_libraries(adbmonitor pico_stdlib)

pico_add_extra_outputs(adbmonitor)

