/**
 * @file ServoController.cpp
 * @brief Implementation of Servo Motor Controller
 */

#include "ServoController.h"

ServoController::ServoController(uint8_t pin, uint16_t min_pulse_us, uint16_t max_pulse_us)
    : pin_(pin), min_pulse_us_(min_pulse_us), max_pulse_us_(max_pulse_us),
      current_angle_(90.0f), target_angle_(90.0f), start_angle_(90.0f),
      move_start_time_(0), move_duration_(0), is_moving_(false), is_attached_(false) {
}

void ServoController::init() {
    // Configure PWM for servo control
    gpio_set_function(pin_, GPIO_FUNC_PWM);
    pwm_slice_ = pwm_gpio_to_slice_num(pin_);
    pwm_channel_ = pwm_gpio_to_channel(pin_);
    
    // Set PWM frequency to 50 Hz (20ms period)
    // Clock speed = 125 MHz
    // For 50 Hz: period = 20ms = 20000µs
    // wrap = (clock_speed / divider / frequency) - 1
    // With divider = 64: wrap = (125000000 / 64 / 50) - 1 = 39061
    float divider = 64.0f;
    uint16_t wrap = 39062;  // Gives exactly 50 Hz
    
    pwm_set_clkdiv(pwm_slice_, divider);
    pwm_set_wrap(pwm_slice_, wrap);
    
    // Set initial position to center (90 degrees)
    setAngle(90.0f);
    
    pwm_set_enabled(pwm_slice_, true);
    is_attached_ = true;
}

void ServoController::setAngle(float angle) {
    // Clamp angle to 0-180
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    
    current_angle_ = angle;
    target_angle_ = angle;
    is_moving_ = false;
    
    if (is_attached_) {
        uint16_t pulse_us = angleToPulseWidth(angle);
        setPulseWidth(pulse_us);
    }
}

void ServoController::moveToAngle(float target_angle, uint32_t duration_ms) {
    // Clamp angle to 0-180
    if (target_angle < 0.0f) target_angle = 0.0f;
    if (target_angle > 180.0f) target_angle = 180.0f;
    
    start_angle_ = current_angle_;
    target_angle_ = target_angle;
    move_start_time_ = to_ms_since_boot(get_absolute_time());
    move_duration_ = duration_ms;
    is_moving_ = true;
}

void ServoController::update() {
    if (!is_moving_) {
        return;
    }
    
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    uint32_t elapsed = current_time - move_start_time_;
    
    if (elapsed >= move_duration_) {
        // Movement complete
        setAngle(target_angle_);
        is_moving_ = false;
    } else {
        // Calculate interpolated angle using linear interpolation
        float progress = (float)elapsed / (float)move_duration_;
        current_angle_ = start_angle_ + (target_angle_ - start_angle_) * progress;
        
        if (is_attached_) {
            uint16_t pulse_us = angleToPulseWidth(current_angle_);
            setPulseWidth(pulse_us);
        }
    }
}

void ServoController::detach() {
    pwm_set_enabled(pwm_slice_, false);
    is_attached_ = false;
}

void ServoController::attach() {
    if (!is_attached_) {
        pwm_set_enabled(pwm_slice_, true);
        is_attached_ = true;
        setAngle(current_angle_);  // Restore current position
    }
}

uint16_t ServoController::angleToPulseWidth(float angle) {
    // Map angle (0-180) to pulse width (min_pulse_us to max_pulse_us)
    uint16_t pulse_us = min_pulse_us_ + 
                        (uint16_t)((angle / 180.0f) * (max_pulse_us_ - min_pulse_us_));
    return pulse_us;
}

void ServoController::setPulseWidth(uint16_t pulse_us) {
    // Calculate PWM level for desired pulse width
    // wrap = 39062, which represents 20000µs
    // level = (pulse_us * wrap) / 20000
    uint16_t level = (pulse_us * 39062) / 20000;
    pwm_set_chan_level(pwm_slice_, pwm_channel_, level);
}
