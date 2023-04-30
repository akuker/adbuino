cmake_minimum_required(VERSION 3.22)

set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/boards) 

# Adapter type should be set to either "QuokkADB" or "adbuinov2"
#   ex: cmake -DADAPTER_TYPE=QuockADB
# This can also be set by environment variable if you're focusing on 
# one adapter type
#   ex: setenv ADAPTER_TYPE=QuockADB
option(ADAPTER_TYPE "ADAPTER_TYPE" QuokkADB)

include(pico_sdk_import.cmake)
# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


string( TOLOWER "${ADAPTER_TYPE}" ADAPTER_TYPE )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Os -DSCQ_RP2040_MUTEX -DRP2040_PLATFORM")

message("-----------------------------------")
if( ADAPTER_TYPE STREQUAL "adbuinov2")
    project(adbuino_v2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DADBUINOV2")
    message("Building for Adbuino v2")
    add_subdirectory(lib/adbuino2/src)
    target_link_libraries(adbuino_v2 pico_stdlib hardware_flash pico_multicore adb usb tinyusb_common tinyusb_host)
else()
    project(QuokkADB)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQUOKKADB")
    message("Building for QuokkADB")
    add_subdirectory(lib/QuokkADB/src)
    target_link_libraries(QuokkADB pico_stdlib hardware_flash pico_multicore adb_rp2040 adb  usb tinyusb_common tinyusb_host)
endif()
message("-----------------------------------")

set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/boards) 

target_include_directories(tinyusb_common INTERFACE include)

include_directories(misc/include adb_rp2040/include adb/include)

add_subdirectory(src)
add_subdirectory(lib/adb/src)
add_subdirectory(lib/adb_rp2040/src)
add_subdirectory(lib/usb/src)

