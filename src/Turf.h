#pragma once
#include "NeonLine.h"
#include "StingRay.h"

struct Pattern;

class Turf {

public:
    Turf();

    void onTick();
    void loop();

private:
    Pattern patterns[];
    int currentPatternIndex;
    int patternStartTime;

    StingRay stingRay;
    NeonLine topWave;
    NeonLine bottomWave;

    int tickState;
    int tickMax;
    float lastTickTime;
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

    void setupAlwaysOn() const;

    void runAlwaysOn() const;
};

struct Pattern {
    void (Turf::*setup)();
    void (Turf::*loop)();

    Pattern(void (Turf::*s)(), void (Turf::*l)())
        : setup(s), loop(l) {}
};

