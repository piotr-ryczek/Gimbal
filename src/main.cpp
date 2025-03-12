#include <Arduino.h>
#include <HardwareSerial.h>
#include <MAVLink.h>

/*
  - ignorujemy YAW
  - Musimy znać pozycje Servo kiedy spogląda w dół
  - Maksymalny wychył nie-ignorowany
  - musimy znać współczynnik ruchy między Servo, a stopniami z Żyroskopu
  - LOOP na 20ms (50Hz): Betaflight domyślnie wysyła co 100ms (10Hz)

  Techniczne:
  - mały regulator 5V
  - ESP32 C3 supermini
  - 2x Servo
  - Rozdzielenie Zasilania i Masy - szybkozłącza 2 x x4
*/

HardwareSerial FC_Serial(1);

void setup() {
  Serial.begin(115200);

  FC_Serial.begin(115200, SERIAL_8N1, 16, 17);
}

float radiansToDegrees(float rad) {
    return rad * (180.0 / PI);
}

void loop() {
  mavlink_message_t msg;
  mavlink_status_t status;

  static uint32_t lastAttitudeTime = 0;
  static uint32_t currentTime = 0;
  static uint32_t timeDiff = 0;

  while (FC_Serial.available()) {
      uint8_t c = FC_Serial.read();

      // Parsuj wiadomości MAVLink
      if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
          if (msg.msgid == MAVLINK_MSG_ID_ATTITUDE) {
            currentTime = millis();
            timeDiff = currentTime - lastAttitudeTime;
            lastAttitudeTime = currentTime;

            Serial.print("Time diff: ");
            Serial.println(timeDiff);

              mavlink_attitude_t attitude;
              mavlink_msg_attitude_decode(&msg, &attitude);
              Serial.print("Roll: ");
              Serial.print(radiansToDegrees(attitude.roll));
              Serial.print(" Pitch: ");
              Serial.print(radiansToDegrees(attitude.pitch));
              Serial.print(" Yaw: ");
              Serial.println(radiansToDegrees(attitude.yaw));
          }
      }
  }
}
