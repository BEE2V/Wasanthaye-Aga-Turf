#include "Turf.h"

#define BODY_RED_PIN 5
#define BODY_GREEN_PIN 6
#define BODY_BLUE_PIN 7
const int BODY_RELAY_PINS[] = {22, 23, 24, 25, 26, 27, 28, 29, 30};
constexpr int BODY_RELAY_COUNT = sizeof(BODY_RELAY_PINS) / sizeof(BODY_RELAY_PINS[0]);

#define LEFT_WING_TOP_PIN 31
#define LEFT_WING_TRIANGLE_PIN 32
#define LEFT_WING_SHORT_PIN 33
#define LEFT_WING_LONG_PIN 34
#define LEFT_WING_CIRCULAR_PIN 35
#define LEFT_WING_RHOMBUS_PIN 36

#define RIGHT_WING_TOP_PIN 37
#define RIGHT_WING_TRIANGLE_PIN 38
#define RIGHT_WING_SHORT_PIN 39
#define RIGHT_WING_LONG_PIN 40
#define RIGHT_WING_CIRCULAR_PIN 41
#define RIGHT_WING_RHOMBUS_PIN 42

#define TAIL_PIN 43

constexpr uint8_t TOP_WAVE_PINS[] = {A0, A1, A2};
constexpr uint8_t BOTTOM_WAVE_PINS[] = {A3, A4, A5};


Turf::Turf() :
    patterns{
        Pattern(&Turf::setupAlwaysOn, &Turf::runAlwaysOn),
        Pattern(&Turf::setupOcean, &Turf::runOcean),
        Pattern(&Turf::setupRainbow, &Turf::runRainbow),
    },
    currentPatternIndex(0),
    patternStartTime(0),
    tickState(0),
    tickMax(120),
    lastTickTime(0),
    delayTime(20)
{
    stingRay = StingRay();

    this->initializeStingray();
    this->initializeWaves();
};

void Turf::onTick() {
    tickState = (tickState + 1) % tickMax;
    const unsigned long now = millis();
    if (lastTickTime != 0) {
        delayTime = delayTime * 0.9 + (now - lastTickTime) * 0.1;
    }
    lastTickTime = now;
    if (tickState == 0) {
        patternStartTime = now;
    }
}

void Turf::loop() {
    this->checkPattern();
    this->patterns[currentPatternIndex].run();
}

void Turf::initializeStingray() {
    // Body
    stingRay.addBodyRGBLine("body", new RGBLine(
        BODY_RED_PIN,
        BODY_GREEN_PIN,
        BODY_BLUE_PIN,
        BODY_RELAY_PINS,
        sizeof(BODY_RELAY_PINS) / sizeof(BODY_RELAY_PINS[0])
    ));

    // Left Wing
    // - Top
    constexpr SegmentSet leftTopSegSet[] = {
        {0, 29}, {0, 29}, {0, 44}, {0, 23},
    };
    stingRay.addLeftWingLEDLine("top", new LEDLine(
        LEFT_WING_TOP_PIN,
        29 + 29 + 44 + 23,
        leftTopSegSet,
        sizeof(leftTopSegSet) / sizeof(leftTopSegSet[0])
    ));
    // - Triangle
    constexpr SegmentSet leftTriangleSegSet[] = {
        {0, 7}, {1, 5},
        {0, 6}, {1, 5},
        {0, 6}, {1, 5},
        {0, 5}, {1, 5},
        {0, 5}, {1, 5},
        {0, 5}, {1, 5},
        {0, 5}, {1, 5},
        {0, 20},
    };
    stingRay.addLeftWingLEDLine("triangle", new LEDLine(
        LEFT_WING_TRIANGLE_PIN,
        7 + 6 + 6 + 5 + 5 + 5 + 5 + 20,
        leftTriangleSegSet,
        sizeof(leftTriangleSegSet) / sizeof(leftTriangleSegSet[0])
    ));
    // - Short
    constexpr SegmentSet leftShortSegSet[] = {
        {0, 27}, {1, 5}, {0, 24}
    };
    stingRay.addLeftWingLEDLine("short", new LEDLine(
        LEFT_WING_SHORT_PIN,
        27 + 24,
        leftShortSegSet,
        sizeof(leftShortSegSet) / sizeof(leftShortSegSet[0])
    ));
    // - Long
    constexpr SegmentSet leftLongSegSet[] = {
        {0, 31}, {1, 5}, {0, 30}, {1, 5}, {0, 27}
    };
    stingRay.addLeftWingLEDLine("long", new LEDLine(
        LEFT_WING_LONG_PIN,
        31 + 30 + 27,
        leftLongSegSet,
        sizeof(leftLongSegSet) / sizeof(leftLongSegSet[0])
    ));
    // - Circular
    constexpr SegmentSet leftCircularSegSet[] = {
        {0, 55}, {1, 5}, {0, 68}
    };
    stingRay.addLeftWingLEDLine("circular", new LEDLine(
        LEFT_WING_CIRCULAR_PIN,
        55 + 68,
        leftCircularSegSet,
        sizeof(leftCircularSegSet) / sizeof(leftCircularSegSet[0])
    ));
    // - Rhombus
    constexpr SegmentSet leftRhombusSegSet[] = {
        {0, 14}, {1, 5},
        {0, 14}, {1, 5},
        {0, 13}, {1, 5},
        {0, 12}, {1, 5},
        {0, 36},
    };
    stingRay.addLeftWingLEDLine("rhombus", new LEDLine(
        LEFT_WING_RHOMBUS_PIN,
        14 + 14 + 13 + 12 + 36,
        leftRhombusSegSet,
        sizeof(leftRhombusSegSet) / sizeof(leftRhombusSegSet[0])
    ));

    // Right Wing
    // - Top
    constexpr SegmentSet rightTopSegSet[] = {
        {0, 30}, {0, 32}, {0, 41}, {0, 24},
    };
    stingRay.addRightWingLEDLine("top", new LEDLine(
        RIGHT_WING_TOP_PIN,
        30 + 32 + 41 + 24,
        rightTopSegSet,
        sizeof(rightTopSegSet) / sizeof(rightTopSegSet[0])
    ));
    // - Triangle
    constexpr SegmentSet rightTriangleSegSet[] = {
        {0, 8}, {1, 5},
        {0, 4}, {1, 5},
        {0, 5}, {1, 5},
        {0, 4}, {1, 5},
        {0, 4}, {1, 5},
        {0, 5}, {1, 5},
        {0, 4}, {1, 5},
        {0, 20}
    };
    stingRay.addRightWingLEDLine("triangle", new LEDLine(
        RIGHT_WING_TRIANGLE_PIN,
        8 + 4 + 5 + 4 + 4 + 5 + 4 + 20,
        rightTriangleSegSet,
        sizeof(rightTriangleSegSet) / sizeof(rightTriangleSegSet[0])
    ));
    // - Short
    constexpr SegmentSet rightShortSegSet[] = {
        {0, 30}, {1, 5}, {0, 29}
    };
    stingRay.addRightWingLEDLine("short", new LEDLine(
        RIGHT_WING_SHORT_PIN,
        30 + 29,
        rightShortSegSet,
        sizeof(rightShortSegSet) / sizeof(rightShortSegSet[0])
    ));
    // - Long
    constexpr SegmentSet rightLongSegSet[] = {
        {0, 34}, {1, 5}, {0, 25}, {1, 5}, {0, 27}
    };
    stingRay.addRightWingLEDLine("long", new LEDLine(
        RIGHT_WING_LONG_PIN,
        34 + 25 + 27,
        rightLongSegSet,
        sizeof(rightLongSegSet) / sizeof(rightLongSegSet[0])
    ));
    // - Circular
    constexpr SegmentSet rightCircularSegSet[] = {
        {0, 56}, {1, 5}, {0, 68}
    };
    stingRay.addRightWingLEDLine("circular", new LEDLine(
        RIGHT_WING_CIRCULAR_PIN,
        56 + 68,
        rightCircularSegSet,
        sizeof(rightCircularSegSet) / sizeof(rightCircularSegSet[0])
    ));
    // - Rhombus
    constexpr SegmentSet rightRhombusSegSet[] = {
        {0, 12}, {1, 5},
        {0, 10}, {1, 5},
        {0, 12}, {1, 5},
        {0, 11}, {1, 5},
        {0, 41},
    };
    stingRay.addRightWingLEDLine("rhombus", new LEDLine(
        RIGHT_WING_RHOMBUS_PIN,
        12 + 10 + 12 + 11 + 41,
        rightRhombusSegSet,
        sizeof(rightRhombusSegSet) / sizeof(rightRhombusSegSet[0])
    ));

    // Tail
    constexpr SegmentSet tailSegSet[] = {
        {0, 120}
    };
    stingRay.setupTail(new LEDLine(
        TAIL_PIN,
        120,
        tailSegSet,
        1
    ));
}

void Turf::initializeWaves() {
    // Waves
    // - Top
    topWave = NeonLine(const_cast<uint8_t*>(TOP_WAVE_PINS), sizeof(TOP_WAVE_PINS) / sizeof(TOP_WAVE_PINS[0]));
    // - Bottom
    bottomWave = NeonLine(const_cast<uint8_t *>(BOTTOM_WAVE_PINS), sizeof(BOTTOM_WAVE_PINS) / sizeof(BOTTOM_WAVE_PINS[0]));
}

void Turf::checkPattern() {
    constexpr int pattern = 0;
    // Determine pattern-based joystick input
    if (pattern != currentPatternIndex) {
        // Switch pattern
        changePattern(pattern);
    }
}

void Turf::changePattern(const int newPatternIndex) {
    currentPatternIndex = newPatternIndex;
    patternStartTime = millis();
    patterns[currentPatternIndex].start();
}

void Turf::setupOcean() {
}

void Turf::runOcean() {
}

void Turf::setupRainbow() {
}

void Turf::runRainbow() {
}

void Turf::setupAlwaysOn() {
    // Turn on all body relays
    if (RGBLine* body = stingRay.getBodyRGBLine("body")) {
        // We don't have a getter for relay count; use known BODY_RELAY_COUNT
        for (int i = 0; i < BODY_RELAY_COUNT; i++) {
            body->updateRelay(i, true);
        }
        body->apply();
    }

    // Solid white for all LED strips on both wings and tail
    uint8_t color[1][3] = { {255, 255, 255} };

    stingRay.getLeftWingLEDLine("top")->runPattern(color, 1);
    stingRay.getLeftWingLEDLine("triangle")->runPattern(color, 1);
    stingRay.getLeftWingLEDLine("short")->runPattern(color, 1);
    stingRay.getLeftWingLEDLine("long")->runPattern(color, 1);
    stingRay.getLeftWingLEDLine("circular")->runPattern(color, 1);
    stingRay.getLeftWingLEDLine("rhombus")->runPattern(color, 1);

    stingRay.getRightWingLEDLine("top")->runPattern(color, 1);
    stingRay.getRightWingLEDLine("triangle")->runPattern(color, 1);
    stingRay.getRightWingLEDLine("short")->runPattern(color, 1);
    stingRay.getRightWingLEDLine("long")->runPattern(color, 1);
    stingRay.getRightWingLEDLine("circular")->runPattern(color, 1);
    stingRay.getRightWingLEDLine("rhombus")->runPattern(color, 1);

    dynamic_cast<LEDLine*>(stingRay.tail)->runPattern(color, 1);

    // Turn on neon waves (top and bottom)
    constexpr int topCount = sizeof(TOP_WAVE_PINS) / sizeof(TOP_WAVE_PINS[0]);
    constexpr int bottomCount = sizeof(BOTTOM_WAVE_PINS) / sizeof(BOTTOM_WAVE_PINS[0]);
    bool topStates[topCount];
    bool bottomStates[bottomCount];
    for (int i = 0; i < topCount; i++) topStates[i] = true;
    for (int i = 0; i < bottomCount; i++) bottomStates[i] = true;
    topWave.runPattern(topStates, topCount);
    bottomWave.runPattern(bottomStates, bottomCount);
}

void Turf::runAlwaysOn() {
    // Keep body relays asserted
    stingRay.getBodyRGBLine("body")->apply();

    // Keep neon waves on
    constexpr int topCount = sizeof(TOP_WAVE_PINS) / sizeof(TOP_WAVE_PINS[0]);
    constexpr int bottomCount = sizeof(BOTTOM_WAVE_PINS) / sizeof(BOTTOM_WAVE_PINS[0]);
    bool topStates[topCount];
    bool bottomStates[bottomCount];
    for (int i = 0; i < topCount; i++) topStates[i] = true;
    for (int i = 0; i < bottomCount; i++) bottomStates[i] = true;
    topWave.runPattern(topStates, topCount);
    bottomWave.runPattern(bottomStates, bottomCount);
}
