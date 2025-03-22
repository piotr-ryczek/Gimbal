#ifndef MEMORY_DATA_H
#define MEMORY_DATA_H

#include <Arduino.h>
#include <memoryValue.h>

// Memory Addresses

const int SERVO_PITCH_NEUTRAL_POSITION_SET_ADDRESS = 0;
const int SERVO_PITCH_NEUTRAL_POSITION_VALUE_ADDRESS = 4;

const int SERVO_ROLL_NEUTRAL_POSITION_SET_ADDRESS = 8;
const int SERVO_ROLL_NEUTRAL_POSITION_VALUE_ADDRESS = 12;

// MemoryValues

// Pitch Neutral
extern MemoryValue servoPitchNeutralPositionMemory;

// Roll Neutral
extern MemoryValue servoRollNeutralPositionMemory;

#endif
