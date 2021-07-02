# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/akuker/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(adbpico C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)

# Add executable. Default name is the project name, version 0.1

add_executable(adbpico adbpico.cpp )

pico_set_program_name(adbpico "adbpico")
pico_set_program_version(adbpico "0.1")

pico_enable_stdio_uart(adbpico 1)
pico_enable_stdio_usb(adbpico 0)

# Add the standard library to the build
target_link_libraries(adbpico pico_stdlib tinyusb_host tinyusb_board)

pico_add_extra_outputs(adbpico)

target_sources(adbpico PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/hid_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adbpico.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/msc_app.c)

target_include_directories(adbpico PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
