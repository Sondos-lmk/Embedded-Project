/**
 * @file PushButton.cpp
 * @brief Implementation of Push Button Driver with Debouncing
 */

#include "PushButton.h"

PushButton::PushButton(uint8_t pin, bool pull_up)
    : pin_(pin), pull_up_(pull_up),
      current_state_(false), last_state_(false), debounced_state_(false),
      last_change_time_(0), press_event_(false), release_event_(false) {
}

void PushButton::init() {
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    
    if (pull_up_) {
        gpio_pull_up(pin_);
    } else {
        gpio_pull_down(pin_);
    }
    
    // Read initial state
    debounced_state_ = readRaw();
    last_state_ = debounced_state_;
    current_state_ = debounced_state_;
    last_change_time_ = to_ms_since_boot(get_absolute_time());
}

bool PushButton::isPressed() {
    return debounced_state_;
}

bool PushButton::wasPressed() {
    if (press_event_) {
        press_event_ = false;
        return true;
    }
    return false;
}

bool PushButton::wasReleased() {
    if (release_event_) {
        release_event_ = false;
        return true;
    }
    return false;
}

void PushButton::update() {
    current_state_ = readRaw();
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    // Check if state has changed
    if (current_state_ != last_state_) {
        last_change_time_ = current_time;
        last_state_ = current_state_;
    }
    
    // Update debounced state if stable for DEBOUNCE_TIME_MS
    if ((current_time - last_change_time_) >= DEBOUNCE_TIME_MS) {
        if (current_state_ != debounced_state_) {
            // State has changed after debounce period
            debounced_state_ = current_state_;
            
            // Set edge events
            if (debounced_state_) {
                press_event_ = true;
            } else {
                release_event_ = true;
            }
        }
    }
}

void PushButton::waitForPress() {
    while (!isPressed()) {
        update();
        sleep_ms(10);
    }
}

void PushButton::waitForRelease() {
    while (isPressed()) {
        update();
        sleep_ms(10);
    }
}

bool PushButton::readRaw() {
    // With pull-up: pressed = LOW (0), released = HIGH (1)
    // Return true when pressed
    if (pull_up_) {
        return !gpio_get(pin_);
    } else {
        return gpio_get(pin_);
    }
}
