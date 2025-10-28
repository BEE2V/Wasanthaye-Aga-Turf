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

    void setupAlwaysOn();

    void runAlwaysOn();
};

struct Pattern {
    void (Turf::*start)();
    void (Turf::*run)();

    Pattern(void (Turf::*s)(), void (Turf::*l)())
        : start(s), run(l) {}
};

