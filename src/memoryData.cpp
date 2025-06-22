#include <Arduino.h>
#include <memoryValue.h>
#include <memoryData.h>

// MemoryValues

// Pitch Neutral
MemoryValue servoPitchNeutralPositionMemory(SERVO_PITCH_NEUTRAL_POSITION_SET_ADDRESS, SERVO_PITCH_NEUTRAL_POSITION_VALUE_ADDRESS);

// Roll Neutral
MemoryValue servoRollNeutralPositionMemory(SERVO_ROLL_NEUTRAL_POSITION_SET_ADDRESS, SERVO_ROLL_NEUTRAL_POSITION_VALUE_ADDRESS);

// Pitch Is Reversed
MemoryValue servoPitchIsReversedMemory(SERVO_PITCH_IS_REVERSED_SET_ADDRESS, SERVO_PITCH_IS_REVERSED_VALUE_ADDRESS, 0);

// Roll Is Reversed
MemoryValue servoRollIsReversedMemory(SERVO_ROLL_IS_REVERSED_SET_ADDRESS, SERVO_ROLL_IS_REVERSED_VALUE_ADDRESS, 0);

// Roll Is Reversed
MemoryValue servosMaxExtremeDiffMemory(SERVOS_MAX_EXTREME_DIFF_SET_ADDRESS, SERVOS_MAX_EXTREME_DIFF_VALUE_ADDRESS, 30);