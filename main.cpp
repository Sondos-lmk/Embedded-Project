/**
 * @file main.cpp
 * @brief Main program for Symbion Station 8 - Puzzle Sorting Robot
 * 
 * This program controls a puzzle-sorting robot using:
 * - 4x4 keypad for user input (select puzzle pieces 1-9)
 * - HC-SR04 ultrasonic sensor for object detection
 * - L298N motor driver + DC motor for linear rail movement
 * - 1 servo motor (MG996) for gripper control
 * - 6 pushbuttons for manual overrides
 * - Active buzzer for audio feedback
 * 
 * Workflow:
 * 1. Startup: Play beep sequence and home the linear rail
 * 2. Wait for keypad input (1-9)
 * 3. Move slider to puzzle piece position
 * 4. Verify presence with ultrasonic sensor
 * 5. Pick up puzzle piece with servos
 * 6. Move to drop-off location
 * 7. Release puzzle piece
 * 8. Confirm with beep
 */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <cmath>

#include "Keypad4x4.h"
#include "Ultrasonic.h"
#include "MotorDriver.h"
#include "ServoController.h"
#include "Buzzer.h"
#include "PushButton.h"
#include "config.h"

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

Keypad4x4 keypad(KEYPAD_ROW_PINS, KEYPAD_COL_PINS);
Ultrasonic ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);
MotorDriver motor(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_ENA_PIN);
ServoController gripperServo(SERVO_GRIPPER_PIN);  // MG996 servo for gripper
// ServoController armServo(SERVO_ARM_PIN);       // NOT USED - only 1 servo available
Buzzer buzzer(BUZZER_PIN);

PushButton stopButton(BUTTON_STOP_PIN);
PushButton homeButton(BUTTON_HOME_PIN);
PushButton manualFwdButton(BUTTON_MANUAL_FWD_PIN);
PushButton manualRevButton(BUTTON_MANUAL_REV_PIN);
PushButton gripButton(BUTTON_GRIP_PIN);
PushButton limitSwitch(BUTTON_LIMIT_PIN);

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

RobotState currentState = STATE_INIT;
float currentPosition = 0.0f;  // Current position in mm
int selectedPuzzlePiece = 0;
bool gripperClosed = false;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void initializeHardware();
void homeLinearRail();
void moveToPosition(float target_position_mm);
bool isAtPosition(float target_position_mm, float tolerance_mm = 2.0f);
void updateButtons();
void updateServos();
void handleManualControl();
void pickupPuzzlePiece();
void releasePuzzlePiece();
void showError(const char* message);
void updateStateMachine();

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    // Initialize standard I/O for debugging
    stdio_init_all();
    sleep_ms(2000);  // Wait for USB serial connection
    
    printf("\n=== Symbion Station 8 - Puzzle Sorting Robot ===\n");
    printf("Initializing...\n");
    
    // Initialize all hardware
    initializeHardware();
    
    // Play startup sequence
    buzzer.playStartupSequence();
    printf("System ready!\n\n");
    
    // Main loop
    while (true) {
        // Update button states
        updateButtons();
        
        // Update servo positions (for smooth motion)
        updateServos();
        
        // Update buzzer patterns
        buzzer.update();
        
        // Check for emergency stop
        if (stopButton.wasPressed()) {
            motor.stop();
            currentState = STATE_IDLE;
            buzzer.playErrorBeep();
            printf("EMERGENCY STOP!\n");
        }
        
        // Check for home button
        if (homeButton.wasPressed()) {
            currentState = STATE_HOMING;
            printf("Homing requested...\n");
        }
        
        // Check for manual control buttons
        if (manualFwdButton.isPressed() || manualRevButton.isPressed() || gripButton.wasPressed()) {
            handleManualControl();
            continue;
        }
        
        // Run state machine
        updateStateMachine();
        
        // Small delay to prevent tight looping
        sleep_ms(10);
    }
    
    return 0;
}

// ============================================================================
// INITIALIZATION
// ============================================================================

void initializeHardware() {
    printf("Initializing keypad...\n");
    keypad.init();
    
    printf("Initializing ultrasonic sensor...\n");
    ultrasonic.init();
    
    printf("Initializing motor driver...\n");
    motor.init();
    
    printf("Initializing servo...\n");
    gripperServo.init();
    // armServo.init();  // NOT USED - only 1 servo available
    
    printf("Initializing buzzer...\n");
    buzzer.init();
    
    printf("Initializing buttons...\n");
    stopButton.init();
    homeButton.init();
    manualFwdButton.init();
    manualRevButton.init();
    gripButton.init();
    limitSwitch.init();
    
    // Set initial servo position
    gripperServo.setAngle(GRIPPER_OPEN_ANGLE);
    // armServo.setAngle(ARM_UP_ANGLE);  // NOT USED - only 1 servo available
    gripperClosed = false;
    
    printf("Hardware initialized successfully!\n");
    
    // Home the linear rail
    currentState = STATE_HOMING;
}

// ============================================================================
// HOMING ROUTINE
// ============================================================================

void homeLinearRail() {
    printf("Homing linear rail...\n");
    
    // Move in reverse until limit switch is triggered
    motor.run(50, MotorDriver::REVERSE);
    
    uint32_t home_start_time = to_ms_since_boot(get_absolute_time());
    
    // Wait for limit switch
    while (!limitSwitch.isPressed()) {
        limitSwitch.update();
        sleep_ms(10);
        
        // Safety timeout (10 seconds)
        if ((to_ms_since_boot(get_absolute_time()) - home_start_time) > 10000) {
            motor.stop();
            showError("Homing timeout!");
            return;
        }
    }
    
    motor.stop();
    currentPosition = HOME_POSITION;
    printf("Homing complete! Position reset to 0.0mm\n");
    buzzer.playConfirmBeep();
}

// ============================================================================
// MOVEMENT FUNCTIONS
// ============================================================================

void moveToPosition(float target_position_mm) {
    float distance = target_position_mm - currentPosition;
    
    if (distance == 0.0f) {
        return;  // Already at position
    }
    
    // Calculate movement time
    uint32_t move_time_ms = (uint32_t)((fabs(distance) / MM_PER_SECOND) * 1000.0f);
    
    // Determine direction
    MotorDriver::Direction direction = (distance > 0) ? MotorDriver::FORWARD : MotorDriver::REVERSE;
    
    printf("Moving %.1fmm (from %.1f to %.1f) in %lums\n", 
           fabs(distance), currentPosition, target_position_mm, move_time_ms);
    
    // Start movement
    motor.moveFor(MOTOR_SPEED, direction, move_time_ms);
    
    // Wait for movement to complete
    while (!motor.isMovementComplete()) {
        updateButtons();
        
        // Check for emergency stop
        if (stopButton.wasPressed()) {
            motor.stop();
            buzzer.playErrorBeep();
            return;
        }
        
        sleep_ms(10);
    }
    
    // Update current position
    currentPosition = target_position_mm;
    printf("Arrived at position %.1fmm\n", currentPosition);
}

bool isAtPosition(float target_position_mm, float tolerance_mm) {
    return fabs(currentPosition - target_position_mm) <= tolerance_mm;
}

// ============================================================================
// BUTTON UPDATE
// ============================================================================

void updateButtons() {
    stopButton.update();
    homeButton.update();
    manualFwdButton.update();
    manualRevButton.update();
    gripButton.update();
    limitSwitch.update();
}

void updateServos() {
    gripperServo.update();
    // armServo.update();  // NOT USED - only 1 servo available
}

// ============================================================================
// MANUAL CONTROL
// ============================================================================

void handleManualControl() {
    if (manualFwdButton.isPressed()) {
        motor.run(MOTOR_SPEED, MotorDriver::FORWARD);
        printf("Manual forward\n");
    } else if (manualRevButton.isPressed()) {
        motor.run(MOTOR_SPEED, MotorDriver::REVERSE);
        printf("Manual reverse\n");
    } else {
        motor.stop();
    }
    
    if (gripButton.wasPressed()) {
        if (gripperClosed) {
            gripperServo.moveToAngle(GRIPPER_OPEN_ANGLE, SERVO_MOVE_TIME_MS);
            gripperClosed = false;
            printf("Gripper opened\n");
        } else {
            gripperServo.moveToAngle(GRIPPER_CLOSED_ANGLE, SERVO_MOVE_TIME_MS);
            gripperClosed = true;
            printf("Gripper closed\n");
        }
        buzzer.playConfirmBeep();
    }
}

// ============================================================================
// PICKUP AND RELEASE
// ============================================================================

void pickupPuzzlePiece() {
    printf("Picking up puzzle piece...\n");
    
    // Note: With only 1 servo (gripper), no arm movement is possible
    // The gripper should be mechanically positioned at the correct height
    
    sleep_ms(PICKUP_DELAY_MS);
    
    // Close gripper
    gripperServo.moveToAngle(GRIPPER_CLOSED_ANGLE, SERVO_MOVE_TIME_MS);
    while (gripperServo.isMoving()) {
        updateServos();
        sleep_ms(10);
    }
    gripperClosed = true;
    sleep_ms(200);
    
    printf("Pickup complete!\n");
    buzzer.playConfirmBeep();
}

void releasePuzzlePiece() {
    printf("Releasing puzzle piece...\n");
    
    // Note: With only 1 servo (gripper), no arm movement is possible
    // The gripper should be mechanically positioned at the correct height
    
    sleep_ms(RELEASE_DELAY_MS);
    
    // Open gripper
    gripperServo.moveToAngle(GRIPPER_OPEN_ANGLE, SERVO_MOVE_TIME_MS);
    while (gripperServo.isMoving()) {
        updateServos();
        sleep_ms(10);
    }
    gripperClosed = false;
    sleep_ms(200);
    
    printf("Release complete!\n");
    buzzer.playSuccessBeep();
}

// ============================================================================
// ERROR HANDLING
// ============================================================================

void showError(const char* message) {
    printf("ERROR: %s\n", message);
    buzzer.playErrorBeep();
    currentState = STATE_ERROR;
}

// ============================================================================
// STATE MACHINE
// ============================================================================

void updateStateMachine() {
    static uint32_t state_start_time = 0;
    
    switch (currentState) {
        case STATE_INIT:
            state_start_time = to_ms_since_boot(get_absolute_time());
            currentState = STATE_HOMING;
            break;
            
        case STATE_HOMING:
            homeLinearRail();
            currentState = STATE_IDLE;
            break;
            
        case STATE_IDLE:
            printf("\n=== Ready for input ===\n");
            printf("Press 1-9 on keypad to select puzzle piece\n");
            printf("Press H button to home\n\n");
            currentState = STATE_WAIT_INPUT;
            break;
            
        case STATE_WAIT_INPUT: {
            char key = keypad.getKey();
            
            if (key >= '1' && key <= '9') {
                selectedPuzzlePiece = key - '0';
                printf("\n>>> Puzzle piece %d selected <<<\n", selectedPuzzlePiece);
                buzzer.playConfirmBeep();
                currentState = STATE_MOVE_TO_PICKUP;
            }
            break;
        }
            
        case STATE_MOVE_TO_PICKUP:
            printf("Moving to pickup position %d...\n", selectedPuzzlePiece);
            moveToPosition(PUZZLE_POSITIONS[selectedPuzzlePiece - 1]);
            currentState = STATE_VERIFY_OBJECT;
            break;
            
        case STATE_VERIFY_OBJECT: {
            printf("Verifying object presence...\n");
            float distance = ultrasonic.measureDistance();
            
            printf("Distance measured: %.2f cm\n", distance);
            
            if (distance < 0) {
                showError("Ultrasonic sensor failure!");
                currentState = STATE_IDLE;
            } else if (distance > OBJECT_DETECTION_THRESHOLD_CM) {
                showError("No puzzle piece detected!");
                printf("Please place puzzle piece %d and press any key to retry\n", selectedPuzzlePiece);
                keypad.waitForKey();
                currentState = STATE_VERIFY_OBJECT;
            } else {
                printf("Object verified! Proceeding to pickup...\n");
                currentState = STATE_PICKUP;
            }
            break;
        }
            
        case STATE_PICKUP:
            pickupPuzzlePiece();
            currentState = STATE_MOVE_TO_DROPOFF;
            break;
            
        case STATE_MOVE_TO_DROPOFF:
            printf("Moving to drop-off location...\n");
            moveToPosition(DROP_OFF_POSITION);
            currentState = STATE_RELEASE;
            break;
            
        case STATE_RELEASE:
            releasePuzzlePiece();
            currentState = STATE_RETURN_HOME;
            break;
            
        case STATE_RETURN_HOME:
            printf("Returning to home position...\n");
            moveToPosition(HOME_POSITION);
            printf("Cycle complete!\n");
            buzzer.playSuccessBeep();
            currentState = STATE_IDLE;
            break;
            
        case STATE_ERROR:
            // Wait for user to press any key to recover
            printf("Press any key to return to idle state...\n");
            keypad.waitForKey();
            currentState = STATE_IDLE;
            break;
            
        case STATE_MANUAL_CONTROL:
            // Handled in main loop
            break;
    }
}
