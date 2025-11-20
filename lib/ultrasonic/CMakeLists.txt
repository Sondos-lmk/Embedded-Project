# Ultrasonic Library CMakeLists.txt

add_library(ultrasonic_lib STATIC
    Ultrasonic.cpp
)

target_include_directories(ultrasonic_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ultrasonic_lib
    pico_stdlib
    hardware_gpio
)
