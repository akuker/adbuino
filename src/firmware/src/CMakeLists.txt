include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(QuokkADB-firmware main.cpp)

target_link_libraries(QuokkADB-firmware QuokkADB)

pico_add_extra_outputs(QuokkADB-firmware)