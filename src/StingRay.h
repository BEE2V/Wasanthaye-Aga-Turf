#ifndef STINGRAY_H
#define STINGRAY_H

#include "LEDLine.h"
#include "RGBLine.h"
#include "LineBase.h"

#define MAX_BODY_LINES 20
#define MAX_WING_LINES 10
#define NAME_LEN 12

struct NamedLine {
    char name[NAME_LEN];
    LineBase* line;
};

class StingRay {
    NamedLine body[MAX_BODY_LINES];
    int bodyCount;

    NamedLine leftWing[MAX_WING_LINES];
    int leftWingCount;

    NamedLine rightWing[MAX_WING_LINES];
    int rightWingCount;

public:
    LineBase* head;
    LineBase* tail;

    StingRay();

    void addBodyLEDLine(const char* name, LEDLine* line);
    void addBodyRGBLine(const char* name, RGBLine* line);

    void addLeftWingLEDLine(const char* name, LEDLine* line);
    void addRightWingLEDLine(const char* name, LEDLine* line);

    LineBase* getBodyLine(const char* name) const;
    LEDLine* getLeftWingLEDLine(const char* name) const;
    LEDLine* getRightWingLEDLine(const char* name) const;
    RGBLine* getBodyRGBLine(const char* name) const;

    void setupTail(LEDLine* tailLine);
    void setupHead(LEDLine* headLine);
};

#endif
