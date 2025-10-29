#include "RGBLine.h"
#include <Arduino.h>

RGBLine::RGBLine(const int red, const int green, const int blue, const int* relayPins, const int count) {
    r = red;
    g = green;
    b = blue;
    relayCount = count > MAX_RELAYS ? MAX_RELAYS : count;

    // Initialize RGB pins for PWM output
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    analogWrite(r, 0);
    analogWrite(g, 0);
    analogWrite(b, 0);

    for (int i = 0; i < relayCount; i++) {
        relays[i] = relayPins[i];
        relayStates[i] = false;
        pinMode(relays[i], OUTPUT);
        // Hardware wired inverse: logical OFF should drive HIGH, ON drives LOW
        digitalWrite(relays[i], HIGH);
    }
}

void RGBLine::setRGBColor(const uint8_t red, const uint8_t green, const uint8_t blue) {
    analogWrite(r, red);
    analogWrite(g, green);
    analogWrite(b, blue);
}

void RGBLine::setRGB(const int red, const int green, const int blue) {
    // Reassign pins (legacy); also configure and reset their outputs
    r = red;
    g = green;
    b = blue;
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
    analogWrite(r, 0);
    analogWrite(g, 0);
    analogWrite(b, 0);
}

void RGBLine::updateRelay(const int index, const bool value) {
    if (index >= 0 && index < relayCount) {
        this->relayStates[index] = value;
    }
}

void RGBLine::updateRelays(const int* indices, const bool* values, const int count) {
    for (int i = 0; i < count; i++) {
        this->updateRelay(indices[i], values[i]);
    }
}

void RGBLine::apply() const {
    for (int i = 0; i < relayCount; i++) {
        // Inverted drive: true => LOW (activate), false => HIGH (deactivate)
        digitalWrite(relays[i], relayStates[i] ? LOW : HIGH);
    }
}

void RGBLine::clear() {
    for (int i = 0; i < relayCount; i++) {
        relayStates[i] = false;
        // Logical OFF => drive HIGH on inverted hardware
        digitalWrite(relays[i], HIGH);
    }
}
