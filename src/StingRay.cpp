#include "StingRay.h"
#include <string.h>

StingRay::StingRay() {
    bodyCount = 0;
    leftWingCount = 0;
    rightWingCount = 0;
    head = nullptr;
    tail = nullptr;
}

// Add body LED line
void StingRay::addBodyLEDLine(const char* name, LEDLine* line) {
    if (bodyCount < MAX_BODY_LINES) {
        strncpy(body[bodyCount].name, name, NAME_LEN);
        body[bodyCount].line = line;
        bodyCount++;
    }
}

// Add body RGB line
void StingRay::addBodyRGBLine(const char* name, RGBLine* line) {
    if (bodyCount < MAX_BODY_LINES) {
        strncpy(body[bodyCount].name, name, NAME_LEN);
        body[bodyCount].line = line;
        bodyCount++;
    }
}

// Add left wing LED line
void StingRay::addLeftWingLEDLine(const char* name, LEDLine* line) {
    if (leftWingCount < MAX_WING_LINES) {
        strncpy(leftWing[leftWingCount].name, name, NAME_LEN);
        leftWing[leftWingCount].line = line;
        leftWingCount++;
    }
}

// Add right wing LED line
void StingRay::addRightWingLEDLine(const char* name, LEDLine* line) {
    if (rightWingCount < MAX_WING_LINES) {
        strncpy(rightWing[rightWingCount].name, name, NAME_LEN);
        rightWing[rightWingCount].line = line;
        rightWingCount++;
    }
}

// Get a generic body line
LineBase* StingRay::getBodyLine(const char* name) const {
    for (int i = 0; i < bodyCount; i++) {
        if (strncmp(body[i].name, name, NAME_LEN) == 0)
            return body[i].line;
    }
    return nullptr;
}

// Get a body RGB line
RGBLine* StingRay::getBodyRGBLine(const char* name) const {
    for (int i = 0; i < bodyCount; i++) {
        if (strncmp(body[i].name, name, NAME_LEN) == 0) {
            // Stored as LineBase*, safe to cast to RGBLine*
            return static_cast<RGBLine*>(body[i].line);
        }
    }
    return nullptr;
}

// Get left wing LED line
LEDLine* StingRay::getLeftWingLEDLine(const char* name) const {
    for (int i = 0; i < leftWingCount; i++) {
        if (strncmp(leftWing[i].name, name, NAME_LEN) == 0) {
            return static_cast<LEDLine*>(leftWing[i].line);
        }
    }
    return nullptr;
}

// Get right wing LED line
LEDLine* StingRay::getRightWingLEDLine(const char* name) const {
    for (int i = 0; i < rightWingCount; i++) {
        if (strncmp(rightWing[i].name, name, NAME_LEN) == 0) {
            return static_cast<LEDLine*>(rightWing[i].line);
        }
    }
    return nullptr;
}

// Setup tail
void StingRay::setupTail(LEDLine* tailLine) {
    tail = tailLine;
}

// Setup head
void StingRay::setupHead(LEDLine* headLine) {
    head = headLine;
}
