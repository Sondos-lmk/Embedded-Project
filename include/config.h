/**
 * @file config.h
 * @brief Configuration file for Symbion Station 8
 * 
 * Contains all pin definitions, constants, and configuration parameters
 * for the puzzle sorting robot.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

// ============================================================================
// PIN DEFINITIONS
// ============================================================================

// Keypad pins (4 rows, 4 columns)
const uint8_t KEYPAD_ROW_PINS[4] = {2, 3, 4, 5};      // GPIO 2-5
const uint8_t KEYPAD_COL_PINS[4] = {6, 7, 8, 9};      // GPIO 6-9

// Ultrasonic sensor pins
const uint8_t ULTRASONIC_TRIGGER_PIN = 10;  // GPIO 10
const uint8_t ULTRASONIC_ECHO_PIN = 11;     // GPIO 11

// Motor driver pins (L298N)
const uint8_t MOTOR_IN1_PIN = 12;   // GPIO 12 - Direction control 1
const uint8_t MOTOR_IN2_PIN = 13;   // GPIO 13 - Direction control 2
const uint8_t MOTOR_ENA_PIN = 14;   // GPIO 14 - PWM speed control

// Servo pins
const uint8_t SERVO_GRIPPER_PIN = 15;    // GPIO 15 - Gripper servo (MG996)
// const uint8_t SERVO_ARM_PIN = 16;     // GPIO 16 - Arm servo (NOT USED)

// Pushbutton pins
const uint8_t BUTTON_STOP_PIN = 17;       // GPIO 17 - Emergency stop
const uint8_t BUTTON_HOME_PIN = 18;       // GPIO 18 - Home position
const uint8_t BUTTON_MANUAL_FWD_PIN = 19; // GPIO 19 - Manual forward
const uint8_t BUTTON_MANUAL_REV_PIN = 20; // GPIO 20 - Manual reverse
const uint8_t BUTTON_GRIP_PIN = 21;       // GPIO 21 - Manual grip
const uint8_t BUTTON_LIMIT_PIN = 22;      // GPIO 22 - Limit switch (home)

// Buzzer pin
const uint8_t BUZZER_PIN = 26;  // GPIO 26

// ============================================================================
// ROBOT CONFIGURATION
// ============================================================================

// Puzzle piece positions (in millimeters from home)
const float PUZZLE_POSITIONS[9] = {
    50.0f,   // Position 1
    100.0f,  // Position 2
    150.0f,  // Position 3
    200.0f,  // Position 4
    250.0f,  // Position 5
    300.0f,  // Position 6
    350.0f,  // Position 7
    400.0f,  // Position 8
    450.0f   // Position 9
};

const float DROP_OFF_POSITION = 500.0f;  // Drop-off location in mm
const float HOME_POSITION = 0.0f;

// Motor calibration (adjust based on your setup)
const float MM_PER_SECOND = 50.0f;  // Linear rail speed
const uint8_t MOTOR_SPEED = 70;      // Motor speed (0-100%)

// Servo positions (MG996 - adjust these values based on your gripper mechanism)
const float GRIPPER_OPEN_ANGLE = 90.0f;      // Open position
const float GRIPPER_CLOSED_ANGLE = 30.0f;    // Closed/gripping position
// const float ARM_UP_ANGLE = 120.0f;        // NOT USED - only 1 servo
// const float ARM_DOWN_ANGLE = 60.0f;       // NOT USED - only 1 servo

// Ultrasonic detection threshold
const float OBJECT_DETECTION_THRESHOLD_CM = 8.0f;

// Timing constants
const uint32_t SERVO_MOVE_TIME_MS = 500;
const uint32_t PICKUP_DELAY_MS = 300;
const uint32_t RELEASE_DELAY_MS = 300;

// ============================================================================
// STATE MACHINE
// ============================================================================

enum RobotState {
    STATE_INIT,
    STATE_HOMING,
    STATE_IDLE,
    STATE_WAIT_INPUT,
    STATE_MOVE_TO_PICKUP,
    STATE_VERIFY_OBJECT,
    STATE_PICKUP,
    STATE_MOVE_TO_DROPOFF,
    STATE_RELEASE,
    STATE_RETURN_HOME,
    STATE_ERROR,
    STATE_MANUAL_CONTROL
};

#endif // CONFIG_H
