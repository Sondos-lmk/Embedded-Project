/**
 * @file Ultrasonic.h
 * @brief HC-SR04 Ultrasonic Sensor Driver for Raspberry Pi Pico
 * 
 * This driver measures distance using the HC-SR04 ultrasonic sensor.
 * Measurement range: 2cm to 400cm
 * Accuracy: ~3mm
 * 
 * Pin Configuration:
 * - Trigger: GPIO output pin
 * - Echo: GPIO input pin
 * 
 * Timing:
 * - Trigger pulse: 10µs
 * - Echo timeout: 30ms (max distance)
 */

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"
#include <cstdint>

class Ultrasonic {
public:
    /**
     * @brief Constructor for HC-SR04 ultrasonic sensor
     * @param trigger_pin GPIO pin for trigger signal
     * @param echo_pin GPIO pin for echo signal
     */
    Ultrasonic(uint8_t trigger_pin, uint8_t echo_pin);
    
    /**
     * @brief Initialize the sensor GPIO pins
     */
    void init();
    
    /**
     * @brief Measure distance in centimeters
     * @return Distance in cm, or -1.0 if measurement failed
     */
    float measureDistance();
    
    /**
     * @brief Check if an object is present within threshold
     * @param threshold_cm Distance threshold in cm
     * @return true if object detected within threshold
     */
    bool isObjectPresent(float threshold_cm = 10.0f);
    
private:
    uint8_t trigger_pin_;
    uint8_t echo_pin_;
    
    static constexpr uint32_t TRIGGER_PULSE_US = 10;
    static constexpr uint32_t TIMEOUT_US = 30000;  // 30ms timeout
    static constexpr float SOUND_SPEED_CM_PER_US = 0.0343f / 2.0f;  // Divided by 2 for round trip
    
    /**
     * @brief Send trigger pulse
     */
    void sendTrigger();
    
    /**
     * @brief Wait for echo pulse and measure duration
     * @return Pulse duration in microseconds, 0 if timeout
     */
    uint32_t measureEchoPulse();
};

#endif // ULTRASONIC_H
