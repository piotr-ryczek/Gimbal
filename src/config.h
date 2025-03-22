#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

extern const int SERVO_PITCH_PWM_TIMER_INDEX;
extern const int SERVO_ROLL_PWM_TIMER_INDEX;

extern const uint8_t SERVO_ROLL_GPIO;
extern const uint8_t SERVO_PITCH_GPIO;

extern bool isBLEClientConnected;

extern const char* BLE_NAME;
extern const char* BLE_SERVICE_UUID;
extern const char* BLE_CHARACTERISTIC_UUID;

#endif
