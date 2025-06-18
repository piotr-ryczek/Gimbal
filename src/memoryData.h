#ifndef MEMORY_DATA_H
#define MEMORY_DATA_H

#include <Arduino.h>
#include <memoryValue.h>

// Memory Addresses

const int SERVO_PITCH_NEUTRAL_POSITION_SET_ADDRESS = 0;
const int SERVO_PITCH_NEUTRAL_POSITION_VALUE_ADDRESS = 4;

const int SERVO_ROLL_NEUTRAL_POSITION_SET_ADDRESS = 8;
const int SERVO_ROLL_NEUTRAL_POSITION_VALUE_ADDRESS = 12;

const int SERVO_PITCH_IS_REVERSED_SET_ADDRESS = 16;
const int SERVO_PITCH_IS_REVERSED_VALUE_ADDRESS = 20;

const int SERVO_ROLL_IS_REVERSED_SET_ADDRESS = 24;
const int SERVO_ROLL_IS_REVERSED_VALUE_ADDRESS = 28;

// MemoryValues

// Pitch Neutral
extern MemoryValue servoPitchNeutralPositionMemory;

// Roll Neutral
extern MemoryValue servoRollNeutralPositionMemory;

// Pitch Is Reversed
extern MemoryValue servoPitchIsReversedMemory;

// Roll Is Reversed
extern MemoryValue servoRollIsReversedMemory;

#endif
