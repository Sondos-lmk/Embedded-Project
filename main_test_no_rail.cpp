/**
 * @file main_test_no_rail.cpp
 * @brief TEST VERSION - Symbion Station 8 WITHOUT Linear Rail
 * 
 * This is a simplified version for testing when the linear rail is not available.
 * It allows you to test:
 * - Keypad input (1-9)
 * - Ultrasonic sensor distance measurement
 * - Servo gripper (open/close)
 * - Buzzer patterns
 * - All pushbuttons
 * 
 * What's disabled:
 * - Motor movement (no rail/slider)
 * - Position tracking
 * - Automated pickup/dropoff sequence
 * 
 * TO USE THE FULL VERSION: Rename main.cpp to main_full.cpp and 
 * rename this file to main.cpp, then rebuild.
 */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>

#include "Keypad4x4.h"
#include "Ultrasonic.h"
// #include "MotorDriver.h"  // Not needed without rail
#include "ServoController.h"
#include "Buzzer.h"
#include "PushButton.h"
#include "config.h"

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

Keypad4x4 keypad(KEYPAD_ROW_PINS, KEYPAD_COL_PINS);
Ultrasonic ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);
// MotorDriver motor(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_ENA_PIN);  // Not used
ServoController gripperServo(SERVO_GRIPPER_PIN);
Buzzer buzzer(BUZZER_PIN);

PushButton stopButton(BUTTON_STOP_PIN);
PushButton gripButton(BUTTON_GRIP_PIN);

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

bool gripperClosed = false;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void initializeHardware();
void testKeypad();
void testUltrasonic();
void testServo();
void testBuzzer();
void showMenu();

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    // Initialize standard I/O for debugging
    stdio_init_all();
    sleep_ms(2000);  // Wait for USB serial connection
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║   Symbion Station 8 - TEST MODE (NO RAIL)        ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");
    printf("This version is for testing WITHOUT the linear rail.\n");
    printf("You can test: Keypad, Ultrasonic, Servo, Buzzer, Buttons\n\n");
    
    // Initialize all hardware
    initializeHardware();
    
    // Play startup sequence
    buzzer.playStartupSequence();
    printf("System ready!\n\n");
    
    showMenu();
    
    // Main loop
    while (true) {
        // Update button states
        stopButton.update();
        gripButton.update();
        
        // Update servo
        gripperServo.update();
        
        // Update buzzer
        buzzer.update();
        
        // Check for grip button
        if (gripButton.wasPressed()) {
            if (gripperClosed) {
                printf("\n>>> Opening gripper...\n");
                gripperServo.moveToAngle(GRIPPER_OPEN_ANGLE, SERVO_MOVE_TIME_MS);
                gripperClosed = false;
            } else {
                printf("\n>>> Closing gripper...\n");
                gripperServo.moveToAngle(GRIPPER_CLOSED_ANGLE, SERVO_MOVE_TIME_MS);
                gripperClosed = true;
            }
            buzzer.playConfirmBeep();
        }
        
        // Check for emergency stop (shows menu)
        if (stopButton.wasPressed()) {
            buzzer.playConfirmBeep();
            showMenu();
        }
        
        // Check for keypad input
        char key = keypad.getKey();
        if (key != '\0') {
            buzzer.playConfirmBeep();
            
            switch (key) {
                case '1':
                    testKeypad();
                    break;
                case '2':
                    testUltrasonic();
                    break;
                case '3':
                    testServo();
                    break;
                case '4':
                    testBuzzer();
                    break;
                case '0':
                    showMenu();
                    break;
                default:
                    printf("\nKey pressed: %c (not assigned)\n", key);
                    printf("Press '0' for menu\n\n");
                    break;
            }
        }
        
        // Small delay
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
    
    // Motor not initialized in test mode
    // printf("Initializing motor driver...\n");
    // motor.init();
    
    printf("Initializing servo...\n");
    gripperServo.init();
    
    printf("Initializing buzzer...\n");
    buzzer.init();
    
    printf("Initializing buttons...\n");
    stopButton.init();
    gripButton.init();
    
    // Set initial servo position
    gripperServo.setAngle(GRIPPER_OPEN_ANGLE);
    gripperClosed = false;
    
    printf("Hardware initialized successfully!\n\n");
}

// ============================================================================
// MENU
// ============================================================================

void showMenu() {
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║              TEST MENU - NO RAIL MODE             ║\n");
    printf("╠════════════════════════════════════════════════════╣\n");
    printf("║  Press on Keypad:                                 ║\n");
    printf("║  [1] - Test Keypad (shows all keys)              ║\n");
    printf("║  [2] - Test Ultrasonic Sensor (continuous)       ║\n");
    printf("║  [3] - Test Servo Gripper (open/close)           ║\n");
    printf("║  [4] - Test Buzzer (all patterns)                ║\n");
    printf("║  [0] - Show this menu                            ║\n");
    printf("║                                                   ║\n");
    printf("║  Buttons:                                         ║\n");
    printf("║  - Grip Button: Toggle gripper open/close        ║\n");
    printf("║  - Stop Button: Show menu                        ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");
}

// ============================================================================
// TEST FUNCTIONS
// ============================================================================

void testKeypad() {
    printf("\n=== KEYPAD TEST ===\n");
    printf("Press any key on the keypad.\n");
    printf("Press '#' to exit this test.\n\n");
    
    while (true) {
        char key = keypad.getKey();
        
        if (key != '\0') {
            if (key == '#') {
                printf("\nExiting keypad test.\n");
                buzzer.playConfirmBeep();
                showMenu();
                return;
            }
            
            printf("Key pressed: [%c]\n", key);
            buzzer.beep(50);
        }
        
        sleep_ms(10);
    }
}

void testUltrasonic() {
    printf("\n=== ULTRASONIC SENSOR TEST ===\n");
    printf("Measuring distance continuously...\n");
    printf("Press any key to exit.\n\n");
    
    uint32_t last_measurement_time = 0;
    
    while (true) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // Measure every 500ms
        if ((current_time - last_measurement_time) >= 500) {
            float distance = ultrasonic.measureDistance();
            
            if (distance < 0) {
                printf("Distance: ERROR (out of range or timeout)\n");
            } else {
                printf("Distance: %.2f cm", distance);
                
                // Visual indicator
                if (distance < 5.0f) {
                    printf(" [VERY CLOSE!]");
                } else if (distance < 10.0f) {
                    printf(" [CLOSE]");
                } else if (distance < 20.0f) {
                    printf(" [NEAR]");
                } else {
                    printf(" [FAR]");
                }
                printf("\n");
            }
            
            last_measurement_time = current_time;
        }
        
        // Check for any key to exit
        if (keypad.getKey() != '\0') {
            printf("\nExiting ultrasonic test.\n");
            buzzer.playConfirmBeep();
            showMenu();
            return;
        }
        
        sleep_ms(10);
    }
}

void testServo() {
    printf("\n=== SERVO GRIPPER TEST ===\n");
    printf("Testing gripper movement...\n\n");
    
    printf("Opening gripper...\n");
    gripperServo.moveToAngle(GRIPPER_OPEN_ANGLE, 1000);
    while (gripperServo.isMoving()) {
        gripperServo.update();
        sleep_ms(10);
    }
    gripperClosed = false;
    buzzer.beep(100);
    sleep_ms(1000);
    
    printf("Closing gripper...\n");
    gripperServo.moveToAngle(GRIPPER_CLOSED_ANGLE, 1000);
    while (gripperServo.isMoving()) {
        gripperServo.update();
        sleep_ms(10);
    }
    gripperClosed = true;
    buzzer.beep(100);
    sleep_ms(1000);
    
    printf("Opening gripper...\n");
    gripperServo.moveToAngle(GRIPPER_OPEN_ANGLE, 1000);
    while (gripperServo.isMoving()) {
        gripperServo.update();
        sleep_ms(10);
    }
    gripperClosed = false;
    buzzer.beep(100);
    
    printf("\nServo test complete!\n");
    buzzer.playSuccessBeep();
    showMenu();
}

void testBuzzer() {
    printf("\n=== BUZZER TEST ===\n");
    
    printf("1. Startup sequence...\n");
    buzzer.playStartupSequence();
    sleep_ms(1000);
    
    printf("2. Confirm beep...\n");
    buzzer.playConfirmBeep();
    sleep_ms(1000);
    
    printf("3. Success beep...\n");
    buzzer.playSuccessBeep();
    sleep_ms(1000);
    
    printf("4. Error beep...\n");
    buzzer.playErrorBeep();
    sleep_ms(1000);
    
    printf("5. Custom pattern (5 short beeps)...\n");
    for (int i = 0; i < 5; i++) {
        buzzer.beep(100);
        sleep_ms(150);
    }
    
    printf("\nBuzzer test complete!\n");
    buzzer.playSuccessBeep();
    showMenu();
}
