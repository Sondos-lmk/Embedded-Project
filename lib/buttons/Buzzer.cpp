/**
 * @file Buzzer.cpp
 * @brief Implementation of Active Buzzer Controller
 */

#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin)
    : pin_(pin), state_(false), pattern_active_(false),
      pattern_count_(0), pattern_current_(0), beep_duration_(0),
      pause_duration_(0), pattern_start_time_(0), pattern_beep_state_(false) {
}

void Buzzer::init() {
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_OUT);
    gpio_put(pin_, 0);
    state_ = false;
}

void Buzzer::on() {
    gpio_put(pin_, 1);
    state_ = true;
}

void Buzzer::off() {
    gpio_put(pin_, 0);
    state_ = false;
}

void Buzzer::beep(uint32_t duration_ms) {
    on();
    sleep_ms(duration_ms);
    off();
}

void Buzzer::playStartupSequence() {
    // Three ascending beeps
    beep(100);
    sleep_ms(100);
    beep(100);
    sleep_ms(100);
    beep(200);
    sleep_ms(100);
}

void Buzzer::playSuccessBeep() {
    // Two short beeps
    beep(100);
    sleep_ms(50);
    beep(100);
}

void Buzzer::playErrorBeep() {
    // Three short rapid beeps
    for (int i = 0; i < 3; i++) {
        beep(150);
        sleep_ms(100);
    }
}

void Buzzer::playConfirmBeep() {
    // Single short beep
    beep(100);
}

void Buzzer::startBeepPattern(uint8_t beep_count, uint32_t beep_duration_ms, uint32_t pause_duration_ms) {
    pattern_count_ = beep_count;
    pattern_current_ = 0;
    beep_duration_ = beep_duration_ms;
    pause_duration_ = pause_duration_ms;
    pattern_start_time_ = to_ms_since_boot(get_absolute_time());
    pattern_beep_state_ = true;
    pattern_active_ = true;
    on();  // Start first beep
}

void Buzzer::update() {
    if (!pattern_active_) {
        return;
    }
    
    updatePattern();
}

void Buzzer::updatePattern() {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    uint32_t elapsed = current_time - pattern_start_time_;
    
    if (pattern_beep_state_) {
        // Currently beeping
        if (elapsed >= beep_duration_) {
            off();
            pattern_beep_state_ = false;
            pattern_start_time_ = current_time;
            pattern_current_++;
            
            // Check if pattern is complete
            if (pattern_current_ >= pattern_count_) {
                pattern_active_ = false;
            }
        }
    } else {
        // Currently in pause
        if (elapsed >= pause_duration_) {
            on();
            pattern_beep_state_ = true;
            pattern_start_time_ = current_time;
        }
    }
}
