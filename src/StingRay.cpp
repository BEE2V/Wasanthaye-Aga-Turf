#include "Stingray.h"
#include <string.h>

StingRay::StingRay() {
    bodyCount = 0;
    leftWingCount = 0;
    rightWingCount = 0;
    tail = nullptr;
}

void StingRay::addBodyLEDLine(const char* name, LEDLine* line) {
    if (bodyCount < MAX_BODY_LINES) {
        strncpy(body[bodyCount].name, name, NAME_LEN);
        body[bodyCount].line = line;
        bodyCount++;
    }
}

void StingRay::addBodyRGBLine(const char* name, RGBLine* line) {
    if (bodyCount < MAX_BODY_LINES) {
        strncpy(body[bodyCount].name, name, NAME_LEN);
        body[bodyCount].line = line;
        bodyCount++;
    }
}

void StingRay::addLeftWingLEDLine(const char* name, LEDLine* line) {
    if (leftWingCount < MAX_WING_LINES) {
        strncpy(leftWing[leftWingCount].name, name, NAME_LEN);
        leftWing[leftWingCount].line = line;
        leftWingCount++;
    }
}

void StingRay::addRightWingLEDLine(const char* name, LEDLine* line) {
    if (rightWingCount < MAX_WING_LINES) {
        strncpy(rightWing[rightWingCount].name, name, NAME_LEN);
        rightWing[rightWingCount].line = line;
        rightWingCount++;
    }
}

LineBase* StingRay::getBodyLine(const char* name) const {
    for (int i = 0; i < bodyCount; i++) {
        if (strncmp(body[i].name, name, NAME_LEN) == 0)
            return body[i].line;
    }
    return nullptr;
}

RGBLine* StingRay::getBodyRGBLine(const char* name) const {
    LineBase* line = getBodyLine(name);
    return dynamic_cast<RGBLine*>(line);
}

LEDLine* StingRay::getLeftWingLEDLine(const char* name) const {
    for (int i = 0; i < leftWingCount; i++) {
        if (strncmp(leftWing[i].name, name, NAME_LEN) == 0)
            return dynamic_cast<LEDLine*>(leftWing[i].line);
    }
    return nullptr;
}

LEDLine* StingRay::getRightWingLEDLine(const char* name) const {
    for (int i = 0; i < rightWingCount; i++) {
        if (strncmp(rightWing[i].name, name, NAME_LEN) == 0)
            return dynamic_cast<LEDLine*>(rightWing[i].line);
    }
    return nullptr;
}

void StingRay::setupTail(LEDLine* tailLine) {
    tail = tailLine;
}
