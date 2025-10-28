#pragma once
#include "NeonLine.h"
#include "StingRay.h"

class Turf;

struct Pattern {
    void (Turf::*start)();
    void (Turf::*run)();

    Pattern() : start(nullptr), run(nullptr) {}
    Pattern(void (Turf::*s)(), void (Turf::*l)())
        : start(s), run(l) {}
};

class Turf {

public:
    Turf();
    void begin();           // <-- new: call after Serial.begin()
    void onTick();
    void loop2();

private:
    Pattern patterns[3];
    int currentPatternIndex;
    unsigned long patternStartTime;

    StingRay stingRay;
    NeonLine* topWave;
    NeonLine* bottomWave;

    int tickState;
    int tickMax;
    unsigned long lastTickTime;
    int delayTime;

    void initializeStingray();

    void initializeWaves();

    void checkPattern();

    void changePattern(int newPatternIndex);

    // Patterns
    void setupOcean();

    void runOcean();

    void setupRainbow();

    void runRainbow();

    void setupAlwaysOn();

    void runAlwaysOn();

    // --- Ocean animation config ---
    uint16_t oceanFrameCount; // how many frames in the first half
    // Order from bottom to top for left wing
    // rhombus, circular, long, short, triangle, top
    struct Gradient { uint8_t start[3]; uint8_t end[3]; };
    Gradient leftWingGradients[6];
    uint16_t leftWingLengths[6];
    void computeLeftWingMeta();
};
