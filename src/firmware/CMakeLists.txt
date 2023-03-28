cmake_minimum_required(VERSION 3.24)

set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/boards) 
#add_definitions(-DPICO_BOARD=quokkadb)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(QuokkADB-firmware)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Os -DQUOKKADB -DSCQ_RP2040_MUTEX ")

target_include_directories(tinyusb_common INTERFACE include)

add_subdirectory(src)
add_subdirectory(lib/adb/src)
add_subdirectory(lib/QuokkADB/src)
add_subdirectory(lib/usb/src)

