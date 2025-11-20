# Keypad Library CMakeLists.txt

add_library(keypad_lib STATIC
    Keypad4x4.cpp
)

target_include_directories(keypad_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(keypad_lib
    pico_stdlib
    hardware_gpio
)
