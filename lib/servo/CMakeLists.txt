# Servo Library CMakeLists.txt

add_library(servo_lib STATIC
    ServoController.cpp
)

target_include_directories(servo_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(servo_lib
    pico_stdlib
    hardware_gpio
    hardware_pwm
)
