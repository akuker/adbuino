

add_library(QuokkADB quokkadb.cpp )

target_link_libraries(QuokkADB pico_stdlib hardware_flash pico_multicore adb adb_rp2040 usb tinyusb_common tinyusb_host)
target_include_directories(QuokkADB PUBLIC ../include)
include_directories(../../misc/include ../../adb_rp2040/include ../../adb/include ../../usb/include)
