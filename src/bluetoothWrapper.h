#ifndef BLUETOOTH_WRAPPER_H
#define BLUETOOTH_WRAPPER_H

#include <vector>
#include <BluetoothSerial.h>
#include <BLEServer.h>

#include <servoWrapper.h>

using namespace std;

class BluetoothWrapper {
  private:
    BLECharacteristic *pCharacteristic;
    ServoWrapper* servoPitch;
    ServoWrapper* servoRoll;

    vector<string> splitString(const String* command);
    string trim(const string& str);

    String handleSetPitchNeutralCommand(uint8_t value);
    String handleSetRollNeutralCommand(uint8_t value);

    String handleSetPitchIsReversedCommand(uint8_t value); // 0 = Normal, 1 = Reversed
    String handleSetRollIsReversedCommand(uint8_t value); // 0 = Normal, 1 = Reversed

    String handleSetMaxExtremeDiffCommand(uint8_t value);

    String handleGetPitchNeutralPositionCommand();
    String handleGetRollNeutralPositionCommand();

    String handleGetMaxExtremeDiffCommand();

    String handleInvalidCommand();

  public:
    BluetoothWrapper(ServoWrapper* servoRoll, ServoWrapper* servoPitch);
    void initialize();
    tuple<vector<String>, String> handleCommand(String* message);
    void checkQueue();
};

#endif