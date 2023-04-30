include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(firmware main.cpp)

if( ADAPTER_TYPE STREQUAL "adbuinov2")
    target_link_libraries(firmware adbuino_v2)
else()
    target_link_libraries(firmware QuokkADB)
endif()

pico_add_extra_outputs(firmware)
