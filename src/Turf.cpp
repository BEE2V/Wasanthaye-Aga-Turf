#include "Turf.h"
#include <Arduino.h>

#define BODY_RED_PIN 5
#define BODY_GREEN_PIN 6
#define BODY_BLUE_PIN 7
const int BODY_RELAY_PINS[] = {22, 23, 24, 25, 26, 27, 28, 29, 30};
constexpr int BODY_RELAY_COUNT = sizeof(BODY_RELAY_PINS) / sizeof(BODY_RELAY_PINS[0]);

#define LEFT_WING_TOP_PIN 11
#define LEFT_WING_TRIANGLE_PIN 13
#define LEFT_WING_SHORT_PIN 9
#define LEFT_WING_LONG_PIN 7
#define LEFT_WING_CIRCULAR_PIN 5
#define LEFT_WING_RHOMBUS_PIN 3

#define RIGHT_WING_TOP_PIN 37
#define RIGHT_WING_TRIANGLE_PIN 38
#define RIGHT_WING_SHORT_PIN 39
#define RIGHT_WING_LONG_PIN 40
#define RIGHT_WING_CIRCULAR_PIN 41
#define RIGHT_WING_RHOMBUS_PIN 42

#define TAIL_PIN 43

constexpr uint8_t TOP_WAVE_PINS[] = {A0, A1, A2};
constexpr uint8_t BOTTOM_WAVE_PINS[] = {A3, A4, A5};


// ------------------------- Turf -------------------------
Turf::Turf() :
    patterns{
        Pattern(&Turf::setupAlwaysOn, &Turf::runAlwaysOn),
        Pattern(&Turf::setupOcean, &Turf::runOcean),
        Pattern(&Turf::setupRainbow, &Turf::runRainbow)
    },
    currentPatternIndex(0),
    patternStartTime(0),
    tickState(0),
    tickMax(120),
    lastTickTime(0),
    topWave(nullptr),
    bottomWave(nullptr),
    delayTime(20)
{
    // Constructor must not perform Serial or hardware initialization
}

// New begin() to run after Serial.begin()
void Turf::begin() {
    Serial.println("Turf: begin");
    initializeStingray();
    initializeWaves();
    Serial.println("Turf: begin complete");
}

// ------------------------- Tick -------------------------
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

// ------------------------- Loop -------------------------
void Turf::loop2() {
    checkPattern();
    // Correct pointer-to-member function call:
    if (patterns[currentPatternIndex].run) {
        (this->*patterns[currentPatternIndex].run)();
    }
}

// ------------------------- Initialization -------------------------
void Turf::initializeStingray() {
    Serial.println("Turf: initializeStingray");

    // Body
    stingRay.addBodyRGBLine("body", new RGBLine(
        BODY_RED_PIN, BODY_GREEN_PIN, BODY_BLUE_PIN,
        BODY_RELAY_PINS, BODY_RELAY_COUNT
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
    Serial.println("Turf: initializeStingray end");
}

void Turf::initializeWaves() {
    Serial.println("Turf: initializeWaves");
    topWave = new NeonLine(const_cast<uint8_t*>(TOP_WAVE_PINS),
                           sizeof(TOP_WAVE_PINS)/sizeof(TOP_WAVE_PINS[0]));
    bottomWave = new NeonLine(const_cast<uint8_t*>(BOTTOM_WAVE_PINS),
                              sizeof(BOTTOM_WAVE_PINS)/sizeof(BOTTOM_WAVE_PINS[0]));
}

void Turf::computeLeftWingMeta() {
    // Resolve lengths of each left wing line (physical LEDs only)
    const char* names[6] = {"rhombus","circular","long","short","triangle","top"};
    for (int i = 0; i < 6; i++) {
        LEDLine* l = stingRay.getLeftWingLEDLine(names[i]);
        leftWingLengths[i] = l ? l->getPhysicalLength() : 0;
    }
}

// ------------------------- Pattern Management -------------------------
void Turf::checkPattern() {
    constexpr int pattern = 1;
    if (pattern != currentPatternIndex) {
        changePattern(pattern);
    }
}

void Turf::changePattern(int newPatternIndex) {
    Serial.print("Turf::changePattern ");
    Serial.print(currentPatternIndex);
    Serial.print(" -> ");
    Serial.println(newPatternIndex);

    currentPatternIndex = newPatternIndex;
    patternStartTime = millis();

    if (patterns[currentPatternIndex].start) {
        (this->*patterns[currentPatternIndex].start)();
    }
}

// ------------------------- Pattern Implementations -------------------------
void Turf::setupOcean() {
    Serial.println("Turf: setupOcean");
    // Turn on body relays
    if (RGBLine* body = stingRay.getBodyRGBLine("body")) {
        for (int i = 0; i < BODY_RELAY_COUNT; i++)
            body->updateRelay(i, false);
        body->apply();
    }

    // Keep LEDs on wings & tail solid white
    uint8_t color[1][3] = {{0, 0, 0}};

    if (auto l = stingRay.getLeftWingLEDLine("top"))       l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("triangle"))  l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("short"))     l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("long"))      l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("circular"))  l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("rhombus"))   l->runPattern(color, 1);

    if (auto r = stingRay.getRightWingLEDLine("top"))       r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("triangle"))  r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("short"))     r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("long"))      r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("circular"))  r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("rhombus"))   r->runPattern(color, 1);

    if (auto tailLed = static_cast<LEDLine*>(stingRay.tail)) tailLed->runPattern(color, 1);

    // Neon waves
    bool topStates[sizeof(TOP_WAVE_PINS)/sizeof(TOP_WAVE_PINS[0])];
    bool bottomStates[sizeof(BOTTOM_WAVE_PINS)/sizeof(BOTTOM_WAVE_PINS[0])];
    for (size_t i = 0; i < (sizeof(topStates)/sizeof(topStates[0])); i++) topStates[i] = false;
    for (size_t i = 0; i < (sizeof(bottomStates)/sizeof(bottomStates[0])); i++) bottomStates[i] = false;
    if (topWave) topWave->runPattern(topStates, (int)(sizeof(topStates)/sizeof(topStates[0])));
    if (bottomWave) bottomWave->runPattern(bottomStates, (int)(sizeof(bottomStates)/sizeof(bottomStates[0])));

    // Ocean config
    // Default gradients per line (can be changed later)
    // rhombus
    leftWingGradients[0] = {{0, 0, 32}, {0, 0, 255}};
    // circular
    leftWingGradients[1] = {{0, 32, 0}, {0, 255, 64}};
    // long
    leftWingGradients[2] = {{0, 32, 32}, {0, 128, 255}};
    // short
    leftWingGradients[3] = {{0, 0, 16}, {0, 128, 200}};
    // triangle
    leftWingGradients[4] = {{0, 16, 32}, {0, 200, 255}};
    // top
    leftWingGradients[5] = {{0, 0, 48}, {0, 255, 255}};

    // Frame count default: sum of physical LEDs across left wing
    computeLeftWingMeta();
    uint32_t total = 0;
    for (int i = 0; i < 6; i++) total += leftWingLengths[i];
    oceanFrameCount = (uint16_t)(total > 65535 ? 65535 : total);
}

void Turf::runOcean() {
    // First half of tick cycle: progressive on; second half: reverse off
    const unsigned long now = millis();
    const unsigned long elapsed = now - patternStartTime;

    // Map elapsed to two halves using tickMax as reference duration unit
    // Use delayTime as approximate ms per tick from onTick smoothing
    const unsigned long cycleMs = (unsigned long)tickMax * (unsigned long)(delayTime > 0 ? delayTime : 1);
    if (cycleMs == 0) return;
    const unsigned long halfMs = cycleMs / 2;
    const unsigned long inCycle = elapsed % cycleMs;
    const bool firstHalf = inCycle < halfMs;

    // Ensure metadata exists
    if (leftWingLengths[0] == 0 && leftWingLengths[1] == 0) {
        computeLeftWingMeta();
    }

    // Fetch left wing lines (bottom to top order)
    LEDLine* rh = stingRay.getLeftWingLEDLine("rhombus");
    LEDLine* ci = stingRay.getLeftWingLEDLine("circular");
    LEDLine* lo = stingRay.getLeftWingLEDLine("long");
    LEDLine* sh = stingRay.getLeftWingLEDLine("short");
    LEDLine* tr = stingRay.getLeftWingLEDLine("triangle");
    LEDLine* tp = stingRay.getLeftWingLEDLine("top");

    LEDLine* lines[6] = {rh, ci, lo, sh, tr, tp};

    if (oceanFrameCount == 0) {
        // Default to sum lengths
        computeLeftWingMeta();
        uint32_t total = 0; for (int i = 0; i < 6; i++) total += leftWingLengths[i];
        oceanFrameCount = (uint16_t)(total > 65535 ? 65535 : total);
        if (oceanFrameCount == 0) return;
    }

    // Calculate frame index within the current half
    const unsigned long posInHalf = firstHalf ? inCycle : (inCycle - halfMs);
    // Use floor mapping (no rounding) to avoid jitter at frame boundaries
    unsigned long numer = posInHalf * (unsigned long)oceanFrameCount;
    uint16_t frameIndex = (uint16_t)(numer / (halfMs ? halfMs : 1));
    if (frameIndex >= oceanFrameCount) frameIndex = oceanFrameCount - 1;

    if (firstHalf) {
        // FIRST HALF: Staggered progressive lighting (bottom to top)
        // Each line starts 1 tick after the previous one
        const unsigned long tickMs = delayTime > 0 ? delayTime : 1;
        
        for (int i = 0; i < 6; i++) {
            LEDLine* L = lines[i];
            if (!L) continue;
            
            // Calculate when this line should start (i ticks after rhombus)
            const unsigned long lineStartMs = i * tickMs;
            
            if (posInHalf < lineStartMs) {
                // This line hasn't started yet - keep it off
                L->clear();
                continue;
            }
            
            // This line is active - calculate its progress
            const unsigned long lineElapsed = posInHalf - lineStartMs;
            const unsigned long lineDuration = halfMs > lineStartMs ? (halfMs - lineStartMs) : 1;
            
            if (lineDuration == 0) {
                L->clear();
                continue;
            }
            
            // Map line progress to frame count for this specific line
            const uint16_t lineFrameCount = leftWingLengths[i] ? leftWingLengths[i] : L->getPhysicalLength();
            const uint16_t lineFrame = (uint16_t)((lineElapsed * lineFrameCount) / lineDuration);
            const uint16_t toDraw = (lineFrame >= lineFrameCount) ? lineFrameCount : lineFrame;
            
            L->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, toDraw);
        }
    } else {
        // SECOND HALF: Reverse progressive turning off (top to bottom)
        // Each line starts turning off 1 tick after the previous one
        const unsigned long tickMs = delayTime > 0 ? delayTime : 1;
        
        for (int i = 5; i >= 0; i--) { // Reverse order: top to bottom
            LEDLine* L = lines[i];
            if (!L) continue;
            
            // Calculate when this line should start turning off (i ticks after top)
            const unsigned long lineStartMs = (5 - i) * tickMs;
            
            if (posInHalf < lineStartMs) {
                // This line hasn't started turning off yet - keep it fully on
                const uint16_t segLen = leftWingLengths[i] ? leftWingLengths[i] : L->getPhysicalLength();
                L->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, segLen);
                continue;
            }
            
            // This line is turning off - calculate its progress
            const unsigned long lineElapsed = posInHalf - lineStartMs;
            const unsigned long lineDuration = halfMs > lineStartMs ? (halfMs - lineStartMs) : 1;
            
            if (lineDuration == 0) {
                L->clear();
                continue;
            }
            
            // Map line progress to remaining LEDs (reverse of lighting up)
            const uint16_t lineFrameCount = leftWingLengths[i] ? leftWingLengths[i] : L->getPhysicalLength();
            const uint16_t lineFrame = (uint16_t)((lineElapsed * lineFrameCount) / lineDuration);
            const uint16_t remaining = (lineFrame >= lineFrameCount) ? 0 : (lineFrameCount - lineFrame);
            
            if (remaining == 0) {
                L->clear();
            } else {
                L->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, remaining);
            }
        }
    }
}

void Turf::setupRainbow() {
    Serial.println("Turf: setupRainbow");
}

void Turf::runRainbow() {
    // Implement your rainbow pattern
}

void Turf::setupAlwaysOn() {
    Serial.println("Turf: setupAlwaysOn");

    // Turn on body relays
    if (RGBLine* body = stingRay.getBodyRGBLine("body")) {
        for (int i = 0; i < BODY_RELAY_COUNT; i++)
            body->updateRelay(i, true);
        body->apply();
    }

    // Keep LEDs on wings & tail solid white
    uint8_t color[1][3] = {{255, 255, 255}};

    if (auto l = stingRay.getLeftWingLEDLine("top"))       l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("triangle"))  l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("short"))     l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("long"))      l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("circular"))  l->runPattern(color, 1);
    if (auto l = stingRay.getLeftWingLEDLine("rhombus"))   l->runPattern(color, 1);

    if (auto r = stingRay.getRightWingLEDLine("top"))       r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("triangle"))  r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("short"))     r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("long"))      r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("circular"))  r->runPattern(color, 1);
    if (auto r = stingRay.getRightWingLEDLine("rhombus"))   r->runPattern(color, 1);

    if (auto tailLed = static_cast<LEDLine*>(stingRay.tail)) tailLed->runPattern(color, 1);

    // Neon waves
    bool topStates[sizeof(TOP_WAVE_PINS)/sizeof(TOP_WAVE_PINS[0])];
    bool bottomStates[sizeof(BOTTOM_WAVE_PINS)/sizeof(BOTTOM_WAVE_PINS[0])];
    for (size_t i = 0; i < (sizeof(topStates)/sizeof(topStates[0])); i++) topStates[i] = true;
    for (size_t i = 0; i < (sizeof(bottomStates)/sizeof(bottomStates[0])); i++) bottomStates[i] = true;
    if (topWave) topWave->runPattern(topStates, (int)(sizeof(topStates)/sizeof(topStates[0])));
    if (bottomWave) bottomWave->runPattern(bottomStates, (int)(sizeof(bottomStates)/sizeof(bottomStates[0])));
}

void Turf::runAlwaysOn() {
    // Keep body relays asserted
    if(auto body = stingRay.getBodyRGBLine("body"))
        body->apply();
    // Keep waves on
    bool topStates[sizeof(TOP_WAVE_PINS)/sizeof(TOP_WAVE_PINS[0])];
    bool bottomStates[sizeof(BOTTOM_WAVE_PINS)/sizeof(BOTTOM_WAVE_PINS[0])];
    for (size_t i = 0; i < (sizeof(topStates)/sizeof(topStates[0])); i++) topStates[i] = true;
    for (size_t i = 0; i < (sizeof(bottomStates)/sizeof(bottomStates[0])); i++) bottomStates[i] = true;
    if (topWave) topWave->runPattern(topStates, (int)(sizeof(topStates)/sizeof(topStates[0])));
    if (bottomWave) bottomWave->runPattern(bottomStates, (int)(sizeof(bottomStates)/sizeof(bottomStates[0])));
}