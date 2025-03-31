#include <Arduino.h>
#include <HardwareSerial.h>
#include <MAVLink.h>
#include <ESP32Servo.h>
#include <EEPROM.h>

#include <servoWrapper.h>
#include <filters.h>
#include <config.h>
#include <memoryData.h>
#include <bluetoothWrapper.h> 
#include <helpers.h>

/*
  - LOOP na 20ms (50Hz): Betaflight domyślnie wysyła co 100ms (10Hz)

  Techniczne:
  - mały regulator 5V
  - ESP32 C3 supermini
  - 2x Servo
  - Rozdzielenie Zasilania i Masy - szybkozłącza 2 x x4
*/

#define EEPROM_SIZE 768

// Task Handles
TaskHandle_t BleTaskHandle;

Servo servoPitch;
Servo servoRoll;

ServoWrapper servoRollWrapper(SERVO_ROLL_GPIO, servoRoll, &servoRollNeutralPositionMemory);
ServoWrapper servoPitchWrapper(SERVO_PITCH_GPIO, servoPitch, &servoPitchNeutralPositionMemory);

BluetoothWrapper bluetoothWrapper(&servoRollWrapper, &servoPitchWrapper);

HardwareSerial FC_Serial(1);

mavlink_message_t msg;
mavlink_status_t status;

void bleTask(void *param) {
  while (true) {
    if (isBLEClientConnected) {
        bluetoothWrapper.checkQueue();

        vTaskDelay(100 / portTICK_PERIOD_MS);
        continue;
    }
  }
  vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void setup() {
  Serial.begin(115200);

  delay(500);
  Serial.println("Starting...");

  if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("EEPROM Error");
      return;
  }

  FC_Serial.begin(115200, SERIAL_8N1, FC_RX_GPIO, FC_TX_GPIO);

  servoPitchWrapper.initialize(SERVO_PITCH_PWM_TIMER_INDEX);
  servoRollWrapper.initialize(SERVO_ROLL_PWM_TIMER_INDEX);

  bluetoothWrapper.initialize();

  Serial.println("Initialized");

  // xTaskCreate(bleTask, "BLETask", 2048, NULL, 1, &BleTaskHandle);
}

void loop() {
  while (FC_Serial.available()) {
      uint8_t c = FC_Serial.read();

      if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
          if (msg.msgid == MAVLINK_MSG_ID_ATTITUDE) {
              mavlink_attitude_t attitude;
              mavlink_msg_attitude_decode(&msg, &attitude);

              auto roll = radiansToDegrees(attitude.roll);
              auto pitch = radiansToDegrees(attitude.pitch);

              Serial.println("Roll: " + String(roll));
              Serial.println("Pitch: " + String(pitch));
 
              float filteredRoll = addValueToBuffer(roll, rollBuffer);
              float filteredPitch = addValueToBuffer(pitch, pitchBuffer);

              // Serial.println("Roll: " + String(filteredRoll));
              // Serial.println("Pitch: " + String(filteredPitch));
              // Serial.println("--------------------------------");

              servoRollWrapper.setDegrees(filteredRoll);
              servoPitchWrapper.setDegrees(filteredPitch);
          }
      }
  }
}
