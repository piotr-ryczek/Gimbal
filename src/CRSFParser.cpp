#include "CRSFParser.h"

void CRSFParser::feed(uint8_t byte) {
  switch (state) {
    case WAIT_START:
      if (byte == CRSF_ADDRESS_FLIGHT_CONTROLLER) {
        state = READ_LENGTH;
      }
      break;

    case READ_LENGTH:
      frameLength = byte;
      payloadIndex = 0;
      state = READ_TYPE;
      break;

    case READ_TYPE:
      frameType = byte;
      state = READ_PAYLOAD;
      break;

    case READ_PAYLOAD:
      payload[payloadIndex++] = byte;
      if (payloadIndex >= frameLength - 2) {
        state = READ_CRC;
      }
      break;

    case READ_CRC:
      crc = byte;
      processFrame();
      state = WAIT_START;
      break;
  }
}

void CRSFParser::processFrame() {
    switch (frameType) {
        case CRSF_TYPE_ATTITUDE: {
            int16_t pitch = (int16_t)((payload[0] << 8) | payload[1]);
            int16_t roll = (int16_t)((payload[2] << 8) | payload[3]);
            int16_t yaw = (int16_t)((payload[4] << 8) | payload[5]);

            // Konwersja na stopnie
            float pitchDeg = radiansToDegrees(pitch * 100.0f / 1000000.0f);
            float rollDeg = radiansToDegrees(roll * 100.0f / 1000000.0f);
            float yawDeg = radiansToDegrees(yaw * 100.0f / 1000000.0f);

            attitude = make_tuple(pitchDeg, rollDeg, yawDeg);
            hasNewAttitudeData = true;

            break;
        }
            
            break;
    }
}

tuple<float, float, float> CRSFParser::getAttitude() {
  return attitude;
}

bool CRSFParser::checkIfHasNewAttitudeData() {
  bool result = hasNewAttitudeData;
  hasNewAttitudeData = false;
  return result;
}