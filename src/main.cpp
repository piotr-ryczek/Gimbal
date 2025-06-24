#include <Arduino.h>
#include <HardwareSerial.h>
#include <CRSFParser.h>
#include <ESP32Servo.h>
#include <EEPROM.h>

#include <servoWrapper.h>
#include <filters.h>
#include <config.h>
#include <memoryData.h>
#include <bluetoothWrapper.h> 
#include <helpers.h>

#define EEPROM_SIZE 768

// Task Handles
TaskHandle_t BleTaskHandle;

Servo servoPitch;
Servo servoRoll;

ServoWrapper servoRollWrapper(SERVO_ROLL_GPIO, servoRoll, &servoRollNeutralPositionMemory, &servoRollIsReversedMemory, &servosMaxExtremeDiffMemory);
ServoWrapper servoPitchWrapper(SERVO_PITCH_GPIO, servoPitch, &servoPitchNeutralPositionMemory, &servoPitchIsReversedMemory, &servosMaxExtremeDiffMemory);

BluetoothWrapper bluetoothWrapper(&servoRollWrapper, &servoPitchWrapper);

HardwareSerial FC_Serial(1);

CRSFParser crsf;

void bleTask(void *param) {
  while (true) {
    if (isBLEClientConnected) {
        bluetoothWrapper.checkQueue();

        vTaskDelay(100 / portTICK_PERIOD_MS);
        continue;
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

Servo servoTest1;
Servo servoTest2;

void setup() {
  Serial.begin(115200);

  delay(500);
  Serial.println("Starting...");

  if (!EEPROM.begin(EEPROM_SIZE)) {
      Serial.println("EEPROM Error");
      return;
  }

  // FC_Serial.begin(115200, SERIAL_8N1, FC_RX_GPIO, FC_TX_GPIO); // 19200
  FC_Serial.begin(420000, SERIAL_8N1, FC_RX_GPIO, FC_TX_GPIO);

  servoPitchWrapper.initialize(SERVO_PITCH_PWM_TIMER_INDEX);
  servoRollWrapper.initialize(SERVO_ROLL_PWM_TIMER_INDEX);

  bluetoothWrapper.initialize();

  Serial.println("Initialized");

  xTaskCreate(bleTask, "BLETask", 4096, NULL, 1, &BleTaskHandle);
}

int value = 0;
void loop() {
  while (FC_Serial.available()) {
    uint8_t byte = FC_Serial.read();
    crsf.feed(byte);

    // Get attitude data from parser
    if (crsf.checkIfHasNewAttitudeData()) {
      auto [pitch, roll, yaw] = crsf.getAttitude();

      float filteredRoll = addValueToBuffer(roll, rollBuffer);
      float filteredPitch = addValueToBuffer(pitch, pitchBuffer);

      // Serial.println("Pitch: " + String(pitch));
      // Serial.println("Roll: " + String(roll));

      servoRollWrapper.setDegrees(filteredRoll);
      servoPitchWrapper.setDegrees(filteredPitch);
    }
  }
}
