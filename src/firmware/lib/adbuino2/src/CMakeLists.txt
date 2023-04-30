
add_library(adbuino_v2 adbuino_v2.cpp)

target_link_libraries(adbuino_v2 pico_stdlib hardware_flash pico_multicore adb adb_rp2040 usb tinyusb_common tinyusb_host)
target_include_directories(adbuino_v2 PUBLIC ../include)

include_directories(../../misc/include ../../adb_rp2040/include ../../adb/include ../../usb/include)