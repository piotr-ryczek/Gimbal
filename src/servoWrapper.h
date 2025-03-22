#ifndef SERVO_WRAPPER_H
#define SERVO_WRAPPER_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <memoryData.h>

const int maxExtremeDiff = 50;

class ServoWrapper {
    private:
        byte servoGpio;
        Servo& servo;
        uint8_t neutralPosition;
        uint8_t min;
        uint8_t max;
        MemoryValue* neutralPositionMemory;
        uint8_t readFromServo();

    public:
        ServoWrapper(byte servoGpio, Servo& servo, MemoryValue* neutralPositionMemory);
        void initialize(int timerNumber);
        void write(uint8_t newPositionDegrees); // Degrees 0 - 180
        void setDegrees(float newPositionDegrees);
        void setNeutralPosition(uint8_t newNeutralPosition);
        void setMinAndMax(uint8_t neutralPosition);

};

#endif