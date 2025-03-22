#include <limits>
#include <iostream>
#include <string>
#include <helpers.h>

float radiansToDegrees(float rad) {
    return rad * (180.0 / PI);
}

uint8_t convertStringToUint8t(string value) {
  if (value.empty()) {
    throw std::invalid_argument("Value is empty");
  }

  int intValue = stoi(value);

  if (intValue < numeric_limits<uint8_t>::min() || intValue > numeric_limits<uint8_t>::max()) {
    throw std::invalid_argument("Number out of range uint8_t");
  }

  return static_cast<uint8_t>(intValue);
}