#include <Arduino.h>
#include <stdexcept>
#include <ESP32Servo.h>

#include <servoWrapper.h>
#include <config.h>

ServoWrapper::ServoWrapper(byte servoGpio, Servo& servo, MemoryValue* neutralPositionMemory, MemoryValue* isReversedMemory, MemoryValue* maxExtremeDiffMemory): servo(servo), neutralPositionMemory(neutralPositionMemory), isReversedMemory(isReversedMemory), maxExtremeDiffMemory(maxExtremeDiffMemory) {
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
    bool isReversed = static_cast<bool>(this->isReversedMemory->readValue());

    int delta = static_cast<int>(round(newPositionDegrees));

    if (isReversed) {
        delta = -delta;
    }

    uint8_t newPosition = static_cast<uint8_t>(this->neutralPosition - delta);

    if (newPosition < this->min) {
        newPosition = this->min;
    } else if (newPosition > this->max) {
        newPosition = this->max;
    }

    Serial.println("New Position: " + String(newPosition));

    this->write(newPosition);
}

uint8_t ServoWrapper::getNeutralPosition() {
    return this->neutralPosition;
}

void ServoWrapper::setNeutralPosition(uint8_t newNeutralPosition) {
    this->neutralPositionMemory->setValue(newNeutralPosition);
    this->neutralPosition = newNeutralPosition;

    this->setMinAndMax(newNeutralPosition);

    if (!hasFCSerialReceivedAnyData) {
        this->setDegrees(newNeutralPosition);
    }
}

void ServoWrapper::setIsReversed(uint8_t newIsReversed) {
    this->isReversedMemory->setValue(newIsReversed);
}

void ServoWrapper::setMinAndMax(uint8_t neutralPosition) {
    uint8_t maxExtremeDiff = maxExtremeDiffMemory->readValue();

    this->min = neutralPosition - maxExtremeDiff;
    this->max = neutralPosition + maxExtremeDiff;
}

void ServoWrapper::recalculateMixAndMax() {
    uint8_t maxExtremeDiff = maxExtremeDiffMemory->readValue();

    this->min = this->neutralPosition - maxExtremeDiff;
    this->max = this->neutralPosition + maxExtremeDiff;
}