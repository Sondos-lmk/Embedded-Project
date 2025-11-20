# Buttons Library CMakeLists.txt

add_library(buttons_lib STATIC
    PushButton.cpp
    Buzzer.cpp
)

target_include_directories(buttons_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(buttons_lib
    pico_stdlib
    hardware_gpio
)
