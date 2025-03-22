#include <Arduino.h>
#include <memoryValue.h>
#include <memoryData.h>

// MemoryValues

// Pitch Neutral
MemoryValue servoPitchNeutralPositionMemory(SERVO_PITCH_NEUTRAL_POSITION_SET_ADDRESS, SERVO_PITCH_NEUTRAL_POSITION_VALUE_ADDRESS);

// Roll Neutral
MemoryValue servoRollNeutralPositionMemory(SERVO_ROLL_NEUTRAL_POSITION_SET_ADDRESS, SERVO_ROLL_NEUTRAL_POSITION_VALUE_ADDRESS);