
add_library(adb_rp2040 
    adb_platform.cpp
    char2usbkeycode.cpp
    flashsettings.cpp
    keyboardrptparser.cpp
    mouserptparser.cpp
    rp2040_gpio.cpp
    rp2040_serial.cpp
    usbhost.cpp
    )


if( ADAPTER_TYPE STREQUAL "adbuinov2")
    include_directories(../../adbuino2/include)
else()
    include_directories(../../QuokkADB/include)

endif()

target_include_directories(adb_rp2040 PUBLIC ../include)
include_directories(../../QuokkADB/include ../../adb/include ../../usb/include)

target_link_libraries(adb_rp2040 pico_stdlib hardware_flash pico_multicore tinyusb_common tinyusb_host)
