/**
 * @file Keypad4x4.h
 * @brief 4x4 Matrix Keypad Driver for Raspberry Pi Pico
 * 
 * This driver handles a 4x4 matrix keypad with debouncing.
 * Keys are numbered 1-9, *, 0, #, and A-D.
 * 
 * Pin Configuration:
 * - Rows: 4 GPIO pins (output)
 * - Columns: 4 GPIO pins (input with pull-up)
 */

#ifndef KEYPAD4X4_H
#define KEYPAD4X4_H

#include "pico/stdlib.h"
#include <cstdint>

class Keypad4x4 {
public:
    /**
     * @brief Constructor for 4x4 keypad
     * @param row_pins Array of 4 GPIO pins for rows
     * @param col_pins Array of 4 GPIO pins for columns
     */
    Keypad4x4(const uint8_t row_pins[4], const uint8_t col_pins[4]);
    
    /**
     * @brief Initialize the keypad GPIO pins
     */
    void init();
    
    /**
     * @brief Scan the keypad for a pressed key (non-blocking)
     * @return Key character if pressed, '\0' if no key pressed
     */
    char getKey();
    
    /**
     * @brief Wait for a key press (blocking)
     * @return The pressed key character
     */
    char waitForKey();
    
private:
    uint8_t row_pins_[4];
    uint8_t col_pins_[4];
    char last_key_;
    uint32_t last_key_time_;
    
    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
    
    // Keypad layout
    static constexpr char KEYS[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    
    /**
     * @brief Scan a single row
     * @param row Row index (0-3)
     * @return Column index if key pressed, -1 otherwise
     */
    int scanRow(uint8_t row);
};

#endif // KEYPAD4X4_H
