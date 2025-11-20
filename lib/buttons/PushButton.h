/**
 * @file PushButton.h
 * @brief Push Button Driver with Debouncing for Raspberry Pi Pico
 * 
 * This driver handles push buttons with software debouncing
 * and edge detection (press/release events).
 * 
 * Pin Configuration:
 * - Button: GPIO input with pull-up resistor
 * - Other side: Ground
 * 
 * Logic:
 * - Pressed: LOW (0)
 * - Released: HIGH (1)
 */

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "pico/stdlib.h"
#include <cstdint>

class PushButton {
public:
    /**
     * @brief Constructor for push button
     * @param pin GPIO pin for button
     * @param pull_up Enable internal pull-up resistor (default true)
     */
    PushButton(uint8_t pin, bool pull_up = true);
    
    /**
     * @brief Initialize button GPIO
     */
    void init();
    
    /**
     * @brief Read current button state (debounced)
     * @return true if button is pressed
     */
    bool isPressed();
    
    /**
     * @brief Check if button was just pressed (rising edge)
     * @return true on press event
     */
    bool wasPressed();
    
    /**
     * @brief Check if button was just released (falling edge)
     * @return true on release event
     */
    bool wasReleased();
    
    /**
     * @brief Update button state (call in main loop)
     * Must be called regularly for debouncing and edge detection
     */
    void update();
    
    /**
     * @brief Wait for button press (blocking)
     */
    void waitForPress();
    
    /**
     * @brief Wait for button release (blocking)
     */
    void waitForRelease();
    
private:
    uint8_t pin_;
    bool pull_up_;
    
    bool current_state_;
    bool last_state_;
    bool debounced_state_;
    
    uint32_t last_change_time_;
    bool press_event_;
    bool release_event_;
    
    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
    
    /**
     * @brief Read raw button state
     * @return Raw GPIO state
     */
    bool readRaw();
};

#endif // PUSHBUTTON_H
