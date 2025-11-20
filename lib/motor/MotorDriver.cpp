/**
 * @file MotorDriver.cpp
 * @brief Implementation of L298N Motor Driver Controller
 */

#include "MotorDriver.h"

MotorDriver::MotorDriver(uint8_t in1_pin, uint8_t in2_pin, uint8_t ena_pin)
    : in1_pin_(in1_pin), in2_pin_(in2_pin), ena_pin_(ena_pin),
      current_speed_(0), current_direction_(BRAKE),
      move_start_time_(0), move_duration_(0), timed_move_active_(false) {
}

void MotorDriver::init() {
    // Configure direction pins as outputs
    gpio_init(in1_pin_);
    gpio_set_dir(in1_pin_, GPIO_OUT);
    gpio_put(in1_pin_, 0);
    
    gpio_init(in2_pin_);
    gpio_set_dir(in2_pin_, GPIO_OUT);
    gpio_put(in2_pin_, 0);
    
    // Configure PWM for speed control
    gpio_set_function(ena_pin_, GPIO_FUNC_PWM);
    pwm_slice_ = pwm_gpio_to_slice_num(ena_pin_);
    pwm_channel_ = pwm_gpio_to_channel(ena_pin_);
    
    // Set PWM frequency
    // Clock speed / (wrap + 1) = frequency
    // 125MHz / (999 + 1) / divider = 1kHz
    // divider = 125
    pwm_set_clkdiv(pwm_slice_, 125.0f);
    pwm_set_wrap(pwm_slice_, PWM_WRAP);
    
    // Start with motor stopped
    pwm_set_chan_level(pwm_slice_, pwm_channel_, 0);
    pwm_set_enabled(pwm_slice_, true);
}

void MotorDriver::setSpeed(uint8_t speed) {
    // Clamp speed to 0-100
    if (speed > 100) {
        speed = 100;
    }
    
    current_speed_ = speed;
    updatePWM();
}

void MotorDriver::setDirection(Direction dir) {
    current_direction_ = dir;
    
    switch (dir) {
        case FORWARD:
            gpio_put(in1_pin_, 1);
            gpio_put(in2_pin_, 0);
            break;
            
        case REVERSE:
            gpio_put(in1_pin_, 0);
            gpio_put(in2_pin_, 1);
            break;
            
        case BRAKE:
            gpio_put(in1_pin_, 0);
            gpio_put(in2_pin_, 0);
            break;
    }
}

void MotorDriver::run(uint8_t speed, Direction dir) {
    setDirection(dir);
    setSpeed(speed);
}

void MotorDriver::stop() {
    setDirection(BRAKE);
    setSpeed(0);
    timed_move_active_ = false;
}

void MotorDriver::moveFor(uint8_t speed, Direction dir, uint32_t duration_ms) {
    run(speed, dir);
    move_start_time_ = to_ms_since_boot(get_absolute_time());
    move_duration_ = duration_ms;
    timed_move_active_ = true;
}

bool MotorDriver::isMovementComplete() {
    if (!timed_move_active_) {
        return true;
    }
    
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - move_start_time_;
    
    if (elapsed >= move_duration_) {
        stop();
        timed_move_active_ = false;
        return true;
    }
    
    return false;
}

void MotorDriver::updatePWM() {
    // Convert speed percentage to PWM level (0-PWM_WRAP)
    uint16_t pwm_level = (current_speed_ * PWM_WRAP) / 100;
    pwm_set_chan_level(pwm_slice_, pwm_channel_, pwm_level);
}
