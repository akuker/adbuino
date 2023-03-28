
add_library(adb
    adb.cpp
    adbkbdparser.cpp
    adbmouseparser.cpp
    usbtoadb.cpp
    )

include_directories(../../misc/include)
target_link_libraries(adb QuokkADB)
target_include_directories(adb PUBLIC ../include)