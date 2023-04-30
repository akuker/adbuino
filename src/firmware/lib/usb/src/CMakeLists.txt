
add_library(usb
    usbkbdparser.cpp
    usbmouseparser.cpp
    )

include_directories(../../misc/include ../../adb_rp2040/include ../../adb/include)
target_link_libraries(usb pico_stdlib tinyusb_common tinyusb_host)
target_include_directories(usb PUBLIC ../include)