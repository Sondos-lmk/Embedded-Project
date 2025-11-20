/**
 * @file Ultrasonic.cpp
 * @brief Implementation of HC-SR04 Ultrasonic Sensor Driver
 */

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t trigger_pin, uint8_t echo_pin)
    : trigger_pin_(trigger_pin), echo_pin_(echo_pin) {
}

void Ultrasonic::init() {
    // Configure trigger pin as output
    gpio_init(trigger_pin_);
    gpio_set_dir(trigger_pin_, GPIO_OUT);
    gpio_put(trigger_pin_, 0);
    
    // Configure echo pin as input
    gpio_init(echo_pin_);
    gpio_set_dir(echo_pin_, GPIO_IN);
    
    sleep_ms(50);  // Allow sensor to stabilize
}

float Ultrasonic::measureDistance() {
    // Send trigger pulse
    sendTrigger();
    
    // Measure echo pulse duration
    uint32_t pulse_duration = measureEchoPulse();
    
    if (pulse_duration == 0) {
        return -1.0f;  // Measurement failed
    }
    
    // Calculate distance: time * speed of sound / 2
    float distance_cm = pulse_duration * SOUND_SPEED_CM_PER_US;
    
    return distance_cm;
}

bool Ultrasonic::isObjectPresent(float threshold_cm) {
    float distance = measureDistance();
    
    if (distance < 0) {
        return false;  // Measurement failed
    }
    
    return distance <= threshold_cm;
}

void Ultrasonic::sendTrigger() {
    // Send 10µs pulse on trigger pin
    gpio_put(trigger_pin_, 0);
    sleep_us(2);
    gpio_put(trigger_pin_, 1);
    sleep_us(TRIGGER_PULSE_US);
    gpio_put(trigger_pin_, 0);
}

uint32_t Ultrasonic::measureEchoPulse() {
    uint32_t timeout_start;
    uint32_t pulse_start;
    uint32_t pulse_end;
    
    // Wait for echo pin to go LOW (in case it's HIGH)
    timeout_start = to_us_since_boot(get_absolute_time());
    while (gpio_get(echo_pin_) == 1) {
        if ((to_us_since_boot(get_absolute_time()) - timeout_start) > TIMEOUT_US) {
            return 0;  // Timeout
        }
    }
    
    // Wait for echo pin to go HIGH (start of pulse)
    timeout_start = to_us_since_boot(get_absolute_time());
    while (gpio_get(echo_pin_) == 0) {
        if ((to_us_since_boot(get_absolute_time()) - timeout_start) > TIMEOUT_US) {
            return 0;  // Timeout
        }
    }
    pulse_start = to_us_since_boot(get_absolute_time());
    
    // Wait for echo pin to go LOW (end of pulse)
    while (gpio_get(echo_pin_) == 1) {
        if ((to_us_since_boot(get_absolute_time()) - pulse_start) > TIMEOUT_US) {
            return 0;  // Timeout
        }
    }
    pulse_end = to_us_since_boot(get_absolute_time());
    
    // Calculate pulse duration
    return pulse_end - pulse_start;
}
