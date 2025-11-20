# Motor Library CMakeLists.txt

add_library(motor_lib STATIC
    MotorDriver.cpp
)

target_include_directories(motor_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(motor_lib
    pico_stdlib
    hardware_gpio
    hardware_pwm
)
