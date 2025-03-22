#include <Arduino.h>
#include <stdexcept>
#include <ESP32Servo.h>
#include <servoWrapper.h>

ServoWrapper::ServoWrapper(byte servoGpio, Servo& servo, MemoryValue* neutralPositionMemory): servo(servo), neutralPositionMemory(neutralPositionMemory) {
    this->servoGpio = servoGpio;
};

void ServoWrapper::initialize(int timerNumber) {
    ESP32PWM::allocateTimer(timerNumber);
    servo.setPeriodHertz(50);
    servo.attach(servoGpio);

    this->neutralPosition = 90;
    if (neutralPositionMemory->isSet()) {
        this->neutralPosition = neutralPositionMemory->readValue();
        this->setMinAndMax(this->neutralPosition);
    } else {
        this->setNeutralPosition(this->neutralPosition);
    }
}

void ServoWrapper::write(uint8_t newPositionDegrees) {
    servo.write(newPositionDegrees);
}

uint8_t ServoWrapper::readFromServo() {
    int value = servo.read();

    if (value < 0) {
        return 0;
    } else if (value > 180) {
        return 180;
    }

    return value;
}

void ServoWrapper::setDegrees(float newPositionDegrees) {
    uint8_t newPosition = static_cast<uint8_t>(this->neutralPosition - round(newPositionDegrees));

    if (newPosition < this->min) {
        newPosition = this->min;
    } else if (newPosition > this->max) {
        newPosition = this->max;
    }

    this->write(newPosition);
}

void ServoWrapper::setNeutralPosition(uint8_t newNeutralPosition) {
    this->neutralPositionMemory->setValue(newNeutralPosition);
    this->neutralPosition = newNeutralPosition;

    this->setMinAndMax(newNeutralPosition);
}

void ServoWrapper::setMinAndMax(uint8_t neutralPosition) {
    this->min = neutralPosition - maxExtremeDiff;
    this->max = neutralPosition + maxExtremeDiff;
}