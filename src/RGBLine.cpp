#include "RGBLine.h"
#include <Arduino.h>

RGBLine::RGBLine(const int red, const int green, const int blue, const int* relayPins, const int count) {
    r = red;
    g = green;
    b = blue;
    relayCount = count > MAX_RELAYS ? MAX_RELAYS : count;

    for (int i = 0; i < relayCount; i++) {
        relays[i] = relayPins[i];
        relayStates[i] = false;
        pinMode(relays[i], OUTPUT);
        digitalWrite(relays[i], LOW);
    }
}

void RGBLine::setRGB(const int red, const int green, const int blue) {
    r = red;
    g = green;
    b = blue;
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
        digitalWrite(relays[i], relayStates[i] ? HIGH : LOW);
    }
}

void RGBLine::clear() {
    for (int i = 0; i < relayCount; i++) {
        relayStates[i] = false;
        digitalWrite(relays[i], LOW);
    }
}
