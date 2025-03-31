#include <config.h>

const int SERVO_PITCH_PWM_TIMER_INDEX = 0;
const int SERVO_ROLL_PWM_TIMER_INDEX = 1;

const uint8_t SERVO_ROLL_GPIO = 1; // 12 on ESP32 "normal"
const uint8_t SERVO_PITCH_GPIO = 2  ; // 14 on ESP32 "normal"

const uint8_t FC_RX_GPIO = 20; // TX on FC // 16 on ESP32 "normal"
const uint8_t FC_TX_GPIO = 21; // RX on FC // 17 on ESP32 "normal"

bool isBLEClientConnected = false;

const char* BLE_NAME = "Gimbal";
const char* BLE_SERVICE_UUID = "591ec76a-b57e-43dc-8e48-467d074e65ea";
const char* BLE_CHARACTERISTIC_UUID = "7549acb0-dd55-4b04-ad89-d3961f43bed8";