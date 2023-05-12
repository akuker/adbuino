
add_library(QuokkADB 
    adb_platform.cpp
    flashsettings.cpp
    platformkbdparser.cpp
    platformmouseparser.cpp
    quokkadb_gpio.cpp
    quokkadb.cpp
    rp2040_serial.cpp
    usbhost.cpp
    )

target_link_libraries(QuokkADB pico_stdlib hardware_flash pico_multicore adb usb tinyusb_common tinyusb_host)
target_include_directories(QuokkADB PUBLIC ../include)