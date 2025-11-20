/**
 * @file Buzzer.h
 * @brief Active Buzzer Controller for Raspberry Pi Pico
 * 
 * This driver controls an active buzzer for audio feedback.
 * Supports beep patterns and tones.
 * 
 * Pin Configuration:
 * - Signal: GPIO output pin
 * - VCC: 5V
 * - GND: Common ground
 * 
 * Note: Active buzzers have built-in oscillator, 
 * so they just need on/off control.
 */

#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include <cstdint>

class Buzzer {
public:
    /**
     * @brief Constructor for buzzer
     * @param pin GPIO pin for buzzer control
     */
    Buzzer(uint8_t pin);
    
    /**
     * @brief Initialize buzzer GPIO
     */
    void init();
    
    /**
     * @brief Turn buzzer on
     */
    void on();
    
    /**
     * @brief Turn buzzer off
     */
    void off();
    
    /**
     * @brief Play a beep for specified duration (blocking)
     * @param duration_ms Beep duration in milliseconds
     */
    void beep(uint32_t duration_ms);
    
    /**
     * @brief Play startup beep sequence
     */
    void playStartupSequence();
    
    /**
     * @brief Play success beep
     */
    void playSuccessBeep();
    
    /**
     * @brief Play error beep sequence
     */
    void playErrorBeep();
    
    /**
     * @brief Play confirmation beep
     */
    void playConfirmBeep();
    
    /**
     * @brief Start a non-blocking beep pattern
     * @param beep_count Number of beeps
     * @param beep_duration_ms Duration of each beep
     * @param pause_duration_ms Pause between beeps
     */
    void startBeepPattern(uint8_t beep_count, uint32_t beep_duration_ms, uint32_t pause_duration_ms);
    
    /**
     * @brief Update beep pattern (call in main loop)
     * Must be called regularly when using startBeepPattern()
     */
    void update();
    
    /**
     * @brief Check if beep pattern is active
     * @return true if pattern is playing
     */
    bool isPlaying() const { return pattern_active_; }
    
private:
    uint8_t pin_;
    bool state_;
    
    // Pattern state
    bool pattern_active_;
    uint8_t pattern_count_;
    uint8_t pattern_current_;
    uint32_t beep_duration_;
    uint32_t pause_duration_;
    uint32_t pattern_start_time_;
    bool pattern_beep_state_;
    
    /**
     * @brief Update pattern state machine
     */
    void updatePattern();
};

#endif // BUZZER_H
