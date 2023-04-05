cmake_minimum_required(VERSION 3.23)

include(pico_sdk_import.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

project(hello_serial)

add_executable(pio_adb)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(pio_adb ${CMAKE_CURRENT_LIST_DIR}/adb_receive.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_sources(pio_adb PRIVATE hello_serial.c)

pico_add_extra_outputs(pio_adb)

# add url via pico_set_program_url
# example_auto_set_url(pio_adb)




# pull in common dependencies
# target_link_libraries(pio_adb pico_stdlib)
target_link_libraries(pio_adb PRIVATE pico_stdlib hardware_pio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DPICO_TIME_DEFAULT_ALARM_POOL_DISABLED=1")


add_custom_target(pio_adb_datasheet DEPENDS ${CMAKE_CURRENT_LIST_DIR}/generated/adb_receive.py)
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/adb_receive.py
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/adb_receive.pio
        COMMAND Pioasm -o python ${CMAKE_CURRENT_LIST_DIR}/adb_receive.pio ${CMAKE_CURRENT_LIST_DIR}/generated/adb_receive.py
        VERBATIM)
add_dependencies(pio_adb pio_adb_datasheet)


# # create map/bin/hex/uf2 file etc.
# pico_add_extra_outputs(hello_serial)

# # enable usb output, disable uart output
# pico_enable_stdio_usb(hello_serial 0)
# pico_enable_stdio_uart(hello_serial 1)

