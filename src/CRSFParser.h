#ifndef CRSF_PARSER_H
#define CRSF_PARSER_H

#include <Arduino.h>
#include <helpers.h>

#define CRSF_ADDRESS_FLIGHT_CONTROLLER 0xC8
#define CRSF_TYPE_ATTITUDE 0x1E

class CRSFParser {
  public:
    void feed(uint8_t byte);
    bool checkIfHasNewAttitudeData();
    tuple<float, float, float> getAttitude();

  private:
    enum State {
      WAIT_START,
      READ_LENGTH,
      READ_TYPE,
      READ_PAYLOAD,
      READ_CRC
    };

    State state = WAIT_START;
    uint8_t frameLength = 0;
    uint8_t frameType = 0;
    uint8_t payload[64];
    uint8_t payloadIndex = 0;
    uint8_t crc = 0;

    bool hasNewAttitudeData = false;
    tuple<float, float, float> attitude;

    void processFrame();
};

#endif