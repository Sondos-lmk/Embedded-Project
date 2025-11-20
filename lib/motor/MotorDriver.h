/**
 * @file MotorDriver.h
 * @brief L298N Motor Driver Controller for Raspberry Pi Pico
 * 
 * This driver controls a DC motor using the L298N H-bridge driver.
 * Supports speed control via PWM and direction control.
 * 
 * Pin Configuration:
 * - IN1: GPIO output (direction control 1)
 * - IN2: GPIO output (direction control 2)
 * - ENA: GPIO PWM output (speed control)
 * 
 * Direction Logic:
 * - Forward: IN1=HIGH, IN2=LOW
 * - Reverse: IN1=LOW, IN2=HIGH
 * - Brake: IN1=LOW, IN2=LOW or IN1=HIGH, IN2=HIGH
 */

#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <cstdint>

class MotorDriver {
public:
    enum Direction {
        FORWARD,
        REVERSE,
        BRAKE
    };
    
    /**
     * @brief Constructor for L298N motor driver
     * @param in1_pin GPIO pin for IN1
     * @param in2_pin GPIO pin for IN2
     * @param ena_pin GPIO PWM pin for ENA (speed control)
     */
    MotorDriver(uint8_t in1_pin, uint8_t in2_pin, uint8_t ena_pin);
    
    /**
     * @brief Initialize motor driver pins and PWM
     */
    void init();
    
    /**
     * @brief Set motor speed
     * @param speed Speed value 0-100 (percentage)
     */
    void setSpeed(uint8_t speed);
    
    /**
     * @brief Set motor direction
     * @param dir Direction (FORWARD, REVERSE, BRAKE)
     */
    void setDirection(Direction dir);
    
    /**
     * @brief Run motor at specified speed and direction
     * @param speed Speed value 0-100
     * @param dir Direction
     */
    void run(uint8_t speed, Direction dir);
    
    /**
     * @brief Stop the motor (brake)
     */
    void stop();
    
    /**
     * @brief Get current speed setting
     * @return Current speed (0-100)
     */
    uint8_t getSpeed() const { return current_speed_; }
    
    /**
     * @brief Get current direction
     * @return Current direction
     */
    Direction getDirection() const { return current_direction_; }
    
    /**
     * @brief Move motor for a specific duration (non-blocking start)
     * @param speed Speed value 0-100
     * @param dir Direction
     * @param duration_ms Duration in milliseconds
     */
    void moveFor(uint8_t speed, Direction dir, uint32_t duration_ms);
    
    /**
     * @brief Check if timed movement is complete
     * @return true if movement is complete or not started
     */
    bool isMovementComplete();
    
private:
    uint8_t in1_pin_;
    uint8_t in2_pin_;
    uint8_t ena_pin_;
    uint8_t current_speed_;
    Direction current_direction_;
    
    uint pwm_slice_;
    uint pwm_channel_;
    
    uint32_t move_start_time_;
    uint32_t move_duration_;
    bool timed_move_active_;
    
    static constexpr uint16_t PWM_WRAP = 999;  // 10-bit resolution
    static constexpr float PWM_FREQ_HZ = 1000.0f;  // 1 kHz PWM frequency
    
    /**
     * @brief Update PWM duty cycle
     */
    void updatePWM();
};

#endif // MOTORDRIVER_H
