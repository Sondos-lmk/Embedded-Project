/**
 * @file ServoController.h
 * @brief Servo Motor Controller for Raspberry Pi Pico
 * 
 * This driver controls standard hobby servo motors (SG90, MG90S, etc.)
 * using PWM signals. Supports smooth motion with micro-stepping.
 * 
 * Pin Configuration:
 * - Signal: GPIO PWM output pin
 * - VCC: 5V (external power recommended)
 * - GND: Common ground
 * 
 * PWM Timing:
 * - Frequency: 50 Hz (20ms period)
 * - Pulse width: 500µs (0°) to 2500µs (180°)
 * - Typical: 1000µs (0°), 1500µs (90°), 2000µs (180°)
 */

#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <cstdint>

class ServoController {
public:
    /**
     * @brief Constructor for servo controller
     * @param pin GPIO PWM pin for servo signal
     * @param min_pulse_us Minimum pulse width in microseconds (default 500)
     * @param max_pulse_us Maximum pulse width in microseconds (default 2500)
     */
    ServoController(uint8_t pin, uint16_t min_pulse_us = 500, uint16_t max_pulse_us = 2500);
    
    /**
     * @brief Initialize servo PWM
     */
    void init();
    
    /**
     * @brief Set servo angle immediately
     * @param angle Angle in degrees (0-180)
     */
    void setAngle(float angle);
    
    /**
     * @brief Move servo to angle smoothly over time
     * @param target_angle Target angle in degrees (0-180)
     * @param duration_ms Duration of movement in milliseconds
     */
    void moveToAngle(float target_angle, uint32_t duration_ms);
    
    /**
     * @brief Update servo position (call in main loop for smooth motion)
     * Must be called regularly when using moveToAngle()
     */
    void update();
    
    /**
     * @brief Check if servo is moving
     * @return true if servo is currently moving to target
     */
    bool isMoving() const { return is_moving_; }
    
    /**
     * @brief Get current angle
     * @return Current angle in degrees
     */
    float getCurrentAngle() const { return current_angle_; }
    
    /**
     * @brief Detach servo (stop PWM signal)
     */
    void detach();
    
    /**
     * @brief Attach servo (resume PWM signal)
     */
    void attach();
    
private:
    uint8_t pin_;
    uint16_t min_pulse_us_;
    uint16_t max_pulse_us_;
    
    float current_angle_;
    float target_angle_;
    float start_angle_;
    
    uint32_t move_start_time_;
    uint32_t move_duration_;
    bool is_moving_;
    bool is_attached_;
    
    uint pwm_slice_;
    uint pwm_channel_;
    
    static constexpr uint16_t PWM_FREQUENCY = 50;  // 50 Hz for servo
    static constexpr uint32_t PWM_PERIOD_US = 20000;  // 20ms period
    
    /**
     * @brief Convert angle to pulse width
     * @param angle Angle in degrees (0-180)
     * @return Pulse width in microseconds
     */
    uint16_t angleToPulseWidth(float angle);
    
    /**
     * @brief Set PWM pulse width
     * @param pulse_us Pulse width in microseconds
     */
    void setPulseWidth(uint16_t pulse_us);
};

#endif // SERVOCONTROLLER_H
