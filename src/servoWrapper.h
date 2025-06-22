#ifndef SERVO_WRAPPER_H
#define SERVO_WRAPPER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <memoryData.h>

class ServoWrapper {
    private:
        byte servoGpio;
        Servo& servo;
        uint8_t neutralPosition;
        uint8_t min;
        uint8_t max;
        MemoryValue* neutralPositionMemory;
        MemoryValue* isReversedMemory;
        MemoryValue* maxExtremeDiffMemory;
        uint8_t readFromServo();

    public:
        ServoWrapper(byte servoGpio, Servo& servo, MemoryValue* neutralPositionMemory, MemoryValue* isReversedMemory, MemoryValue* maxExtremeDiffMemory);
        void initialize(int timerNumber);
        void write(uint8_t newPositionDegrees); // Degrees 0 - 180
        void setDegrees(float newPositionDegrees);
        uint8_t getNeutralPosition();
        void setNeutralPosition(uint8_t newNeutralPosition);
        void setIsReversed(uint8_t newIsReversed); // 0 = Normal, 1 = Reversed
        void setMinAndMax(uint8_t neutralPosition);
        void recalculateMixAndMax();

};

#endif