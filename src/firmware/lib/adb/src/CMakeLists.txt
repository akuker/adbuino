
add_library(adb
    adb.cpp
    adbkbdparser.cpp
    adbmouseparser.cpp
    usbtoadb.cpp
    )

include_directories(../../misc/include ../../adb_rp2040/include ../../usb/include)
target_link_libraries(adb pico_stdlib tinyusb_common tinyusb_host)
target_include_directories(adb PUBLIC ../include)