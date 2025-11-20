/**
 * @file Keypad4x4.cpp
 * @brief Implementation of 4x4 Matrix Keypad Driver
 */

#include "Keypad4x4.h"
#include <cstring>

Keypad4x4::Keypad4x4(const uint8_t row_pins[4], const uint8_t col_pins[4])
    : last_key_('\0'), last_key_time_(0) {
    // Copy pin arrays
    memcpy(row_pins_, row_pins, 4 * sizeof(uint8_t));
    memcpy(col_pins_, col_pins, 4 * sizeof(uint8_t));
}

void Keypad4x4::init() {
    // Configure row pins as outputs (initially HIGH)
    for (int i = 0; i < 4; i++) {
        gpio_init(row_pins_[i]);
        gpio_set_dir(row_pins_[i], GPIO_OUT);
        gpio_put(row_pins_[i], 1);
    }
    
    // Configure column pins as inputs with pull-up resistors
    for (int i = 0; i < 4; i++) {
        gpio_init(col_pins_[i]);
        gpio_set_dir(col_pins_[i], GPIO_IN);
        gpio_pull_up(col_pins_[i]);
    }
    
    sleep_ms(10);  // Allow pins to stabilize
}

char Keypad4x4::getKey() {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    // Scan all rows
    for (int row = 0; row < 4; row++) {
        int col = scanRow(row);
        
        if (col != -1) {
            char key = KEYS[row][col];
            
            // Debounce: only register if enough time has passed since last key
            if (key != last_key_ || (current_time - last_key_time_) > DEBOUNCE_TIME_MS) {
                last_key_ = key;
                last_key_time_ = current_time;
                return key;
            }
            return '\0';  // Still in debounce period
        }
    }
    
    // No key pressed
    last_key_ = '\0';
    return '\0';
}

char Keypad4x4::waitForKey() {
    char key = '\0';
    
    // Wait until no key is pressed
    while (getKey() != '\0') {
        sleep_ms(10);
    }
    
    // Wait for a key press
    while (key == '\0') {
        key = getKey();
        sleep_ms(10);
    }
    
    return key;
}

int Keypad4x4::scanRow(uint8_t row) {
    // Set all rows HIGH
    for (int i = 0; i < 4; i++) {
        gpio_put(row_pins_[i], 1);
    }
    
    // Set current row LOW
    gpio_put(row_pins_[row], 0);
    
    // Small delay for signal to stabilize
    sleep_us(10);
    
    // Check each column
    for (int col = 0; col < 4; col++) {
        if (gpio_get(col_pins_[col]) == 0) {
            // Key pressed at this row/column
            // Reset all rows to HIGH before returning
            for (int i = 0; i < 4; i++) {
                gpio_put(row_pins_[i], 1);
            }
            return col;
        }
    }
    
    // No key pressed in this row
    return -1;
}
