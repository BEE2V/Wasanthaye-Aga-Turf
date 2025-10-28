#include "NeonLine.h"
#include <Arduino.h>

NeonLine::NeonLine(uint8_t* pins, const int pinCount) {
    this->pins = pins;
    this->pinCount = pinCount;

    for (int i = 0; i < pinCount; i++) {
        pinMode(pins[i], OUTPUT);
        analogWrite(pins[i], LOW);
    }
}

void NeonLine::runPattern(const bool* states, const int stateCount) const {
    for (int i = 0; i < pinCount; i++) {
        const bool state = (i < stateCount) ? states[i] : false;
        analogWrite(pins[i], state ? HIGH : LOW);
    }
}

void NeonLine::clear() const {
    for (int i = 0; i < pinCount; i++) {
        analogWrite(pins[i], LOW);
    }
}
