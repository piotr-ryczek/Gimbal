#include <Arduino.h>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <bluetoothWrapper.h>
#include <memoryData.h>
#include <config.h>
#include <helpers.h>
using namespace std;

vector<String> BLEQueue;
unordered_map<string, MemoryValue*> settingsMemory;
vector<string> commandTypes = {
  "SET_PITCH_NEUTRAL_POSITION",
  "SET_ROLL_NEUTRAL_POSITION",
};

BluetoothWrapper::BluetoothWrapper(ServoWrapper* servoRoll, ServoWrapper* servoPitch): servoRoll(servoRoll), servoPitch(servoPitch) {}

class WindowOpeningBLEServerCallbacks : public BLEServerCallbacks {
  public:
    void onConnect(BLEServer* pServer) override {
        isBLEClientConnected = true;
        Serial.println("Client connected");
    }

    void onDisconnect(BLEServer* pServer) override {
      isBLEClientConnected = false;
      pServer->startAdvertising();
      Serial.println("Client disconnected");
    }
};

class WindowOpeningBLECharacteristicCallbacks : public BLECharacteristicCallbacks {
  private:
    BluetoothWrapper* bluetoothWrapper;

  public:
    WindowOpeningBLECharacteristicCallbacks(BluetoothWrapper* wrapper) : bluetoothWrapper(wrapper) {}

    WindowOpeningBLECharacteristicCallbacks() {}

    void onWrite(BLECharacteristic* pCharacteristic) override {
      if (!bluetoothWrapper) {
          Serial.println("BluetoothWrapper is not initialized");
          return;
      }

      std::string value = pCharacteristic->getValue();
      Serial.print("BLE Received: ");
      Serial.println(value.c_str());

      value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
      value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());

      String valueString = value.c_str();
      auto [response, commandType] = bluetoothWrapper->handleCommand(&valueString);

      for (const auto& item : response) {
        BLEQueue.push_back(item);
      }
    }
};

void BluetoothWrapper::initialize() {
  BLEDevice::init(BLE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
  this->pCharacteristic = pService->createCharacteristic(BLE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_INDICATE | BLECharacteristic::PROPERTY_NOTIFY);

  this->pCharacteristic->addDescriptor(new BLE2902());

  pServer->setCallbacks(new WindowOpeningBLEServerCallbacks());
  this->pCharacteristic->setCallbacks(new WindowOpeningBLECharacteristicCallbacks(this));
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  settingsMemory["SERVO_PITCH_NEUTRAL_POSITION"] = &servoPitchNeutralPositionMemory;
  settingsMemory["SERVO_ROLL_NEUTRAL_POSITION"] = &servoRollNeutralPositionMemory;

  Serial.println("Bluetooth initialized. Ready for pairing");
}

void BluetoothWrapper::checkQueue() {
  if (BLEQueue.empty()) return;

  String firstMessage = BLEQueue.front();
  const char* firstMessageChar = firstMessage.c_str();

  Serial.println("Notify BLE with: ");
  Serial.println(firstMessageChar);

  this->pCharacteristic->setValue(firstMessageChar);
  this->pCharacteristic->notify();
  BLEQueue.erase(BLEQueue.begin());
}

tuple<vector<String>, String> BluetoothWrapper::handleCommand(String* message) {
  Serial.print("Bluetooth data received: ");
  Serial.println(message->c_str());
  
  vector<String> response;
  vector<string> parts = this->splitString(message);

  if (parts.empty()) {
    Serial.println("Not processing bluetooth command: zero parts");
    response.push_back("Invalid Command: Zero Parts");
}

  if (parts.size() != 2) {
    Serial.println("Not processing bluetooth command: only two parts accepted");
    response.push_back("Invalid Command: Only two parts accepted");
  }

  string commandType = trim(parts.at(0));
  uint8_t newServosPosition;

  if (commandType == "SET_PITCH_NEUTRAL_POSITION" || commandType == "SET_ROLL_NEUTRAL_POSITION") {
    newServosPosition = convertStringToUint8t(trim(parts.at(1)));
  }

  if (commandType == "SET_PITCH_NEUTRAL_POSITION") {
    response.push_back(handleSetPitchNeutralCommand(newServosPosition));
  } else if (commandType == "SET_ROLL_NEUTRAL_POSITION") {
    response.push_back(handleSetRollNeutralCommand(newServosPosition));
  } else {
    response.push_back(handleInvalidCommand());
  }

  return make_tuple(response, commandType.c_str());
}

vector<string> BluetoothWrapper::splitString(const String* command) {
  vector<string> parts;
  stringstream ss(command->c_str());
  string token;

  while (getline(ss, token, ' ')) {
    parts.push_back(token);
  }

  if (parts.size() < 1 || parts.size() > 3) {
    Serial.println("Invalid command: parts less than 1 or more than 3");

    return vector<string>();
  }

  string commandType = trim(parts.at(0));

  auto it = find(commandTypes.begin(), commandTypes.end(), commandType);

  if (it == commandTypes.end()) {
    Serial.println("Invalid command: invalid command type");

    return vector<string>();
  }

  if (
    (commandType == "SET_PITCH_NEUTRAL_POSITION" || commandType == "SET_ROLL_NEUTRAL_POSITION")
    && parts.size() != 2
  ) {
    Serial.println("Invalid command: Command expects to be two parts");

    return vector<string>();
  }

  return parts;
}

string BluetoothWrapper::trim(const string& str) {
  size_t first = str.find_first_not_of(" \t\n\r");
  size_t last = str.find_last_not_of(" \t\n\r");
  return (first == string::npos || last == string::npos) ? "" : str.substr(first, (last - first + 1));
}

String BluetoothWrapper::handleSetPitchNeutralCommand(uint8_t value) {
  servoPitch->setNeutralPosition(value);

  return "Set Pitch Neutral Position correctly";
}

String BluetoothWrapper::handleSetRollNeutralCommand(uint8_t value) {
  servoRoll->setNeutralPosition(value);

  return "Set Roll Neutral Position correctly";
}

String BluetoothWrapper::handleInvalidCommand() {
  Serial.println("Invalid command");
  return "Invalid command";
}