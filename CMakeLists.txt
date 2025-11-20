# CMakeLists.txt for Symbion Station 8 - Puzzle Sorting Robot
# Raspberry Pi Pico RP2040 Project

cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include(pico_sdk_import.cmake)

# Project name and languages
project(symbion_station8 C CXX ASM)

# Set C++ standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/keypad)
include_directories(${CMAKE_SOURCE_DIR}/lib/ultrasonic)
include_directories(${CMAKE_SOURCE_DIR}/lib/motor)
include_directories(${CMAKE_SOURCE_DIR}/lib/servo)
include_directories(${CMAKE_SOURCE_DIR}/lib/buttons)

# Add library subdirectories
add_subdirectory(lib/keypad)
add_subdirectory(lib/ultrasonic)
add_subdirectory(lib/motor)
add_subdirectory(lib/servo)
add_subdirectory(lib/buttons)

# Add executable
add_executable(${PROJECT_NAME}
    main.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_gpio
    hardware_pwm
    hardware_timer
    hardware_irq
    keypad_lib
    ultrasonic_lib
    motor_lib
    servo_lib
    buttons_lib
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
