#include "Turf.h"
#include <Arduino.h>

#define BODY_RED_PIN 5
#define BODY_GREEN_PIN 6
#define BODY_BLUE_PIN 7
constexpr int BODY_RELAY_PINS[] = {A12, A15, A13, A14, A11, A10, A8, A9, A7};
constexpr int BODY_RELAY_COUNT = sizeof(BODY_RELAY_PINS) / sizeof(BODY_RELAY_PINS[0]);

#define LEFT_WING_TOP_PIN 24
#define LEFT_WING_TRIANGLE_PIN 21
#define LEFT_WING_SHORT_PIN 16
#define LEFT_WING_LONG_PIN 17
#define LEFT_WING_CIRCULAR_PIN 14
#define LEFT_WING_RHOMBUS_PIN 15

#define RIGHT_WING_TOP_PIN 23
#define RIGHT_WING_TRIANGLE_PIN 22
#define RIGHT_WING_SHORT_PIN 18
#define RIGHT_WING_LONG_PIN 19
#define RIGHT_WING_CIRCULAR_PIN 20
#define RIGHT_WING_RHOMBUS_PIN 25

#define HEAD_PIN 27
#define TAIL_PIN 26

constexpr uint8_t TOP_WAVE_PINS[] = {A3, A4, A5};
constexpr uint8_t BOTTOM_WAVE_PINS[] = {A0, A1, A2};


// ------------------------- Turf -------------------------
Turf::Turf() :
    patterns{
        Pattern(&Turf::setupAlwaysOn, &Turf::runAlwaysOn),
        Pattern(&Turf::setupOcean, &Turf::runOcean),
        Pattern(&Turf::setupRainbow, &Turf::runRainbow)
    },
    currentPatternIndex(-1),
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

    // Head
    constexpr SegmentSet headSegSet[] = {
        {0, 24}
    };
    stingRay.setupHead(new LEDLine(
        HEAD_PIN,
        24,
        headSegSet,
        1
    ));

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
        28 + 29 + 44 + 23,
        leftTopSegSet,
        sizeof(leftTopSegSet) / sizeof(leftTopSegSet[0])
    ));
    // - Triangle
    constexpr SegmentSet leftTriangleSegSet[] = {
        {0, 7}, {1, 5},
        {0, 6}, {1, 5},
        {0, 7}, {1, 5},
        {0, 5}, {1, 5},
        {0, 4}, {1, 5},
        {0, 5}, {1, 5},
        {0, 5}, {1, 5},
        {0, 20},
    };
    stingRay.addLeftWingLEDLine("triangle", new LEDLine(
        LEFT_WING_TRIANGLE_PIN,
        7 + 6 + 7 + 5 + 4 + 5 + 5 + 20,
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
        {0, 30}, {1, 5}, {0, 30}, {1, 5}, {0, 27}
    };
    stingRay.addLeftWingLEDLine("long", new LEDLine(
        LEFT_WING_LONG_PIN,
        30 + 30 + 27,
        leftLongSegSet,
        sizeof(leftLongSegSet) / sizeof(leftLongSegSet[0])
    ));
    // - Circular
    constexpr SegmentSet leftCircularSegSet[] = {
        {0, 56}, {1, 5}, {0, 68}
    };
    stingRay.addLeftWingLEDLine("circular", new LEDLine(
        LEFT_WING_CIRCULAR_PIN,
        56 + 68,
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
        {0, 30}, {0, 31}, {0, 41}, {0, 24},
    };
    stingRay.addRightWingLEDLine("top", new LEDLine(
        RIGHT_WING_TOP_PIN,
        30 + 31 + 41 + 24,
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
        {0, 122}
    };
    stingRay.setupTail(new LEDLine(
        TAIL_PIN,
        122,
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
        // Set an oceanic base color on the body motor driver (RGB pins)
        body->setRGBColor(0, 160, 255);
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
    if (auto headLed = static_cast<LEDLine*>(stingRay.head)) headLed->runPattern(color, 1);

    // Neon waves: initialize with first pin ON to start the chase
    bool topStates[sizeof(TOP_WAVE_PINS)/sizeof(TOP_WAVE_PINS[0])];
    bool bottomStates[sizeof(BOTTOM_WAVE_PINS)/sizeof(BOTTOM_WAVE_PINS[0])];
    for (size_t i = 0; i < (sizeof(topStates)/sizeof(topStates[0])); i++) topStates[i] = (i == 0);
    for (size_t i = 0; i < (sizeof(bottomStates)/sizeof(bottomStates[0])); i++) bottomStates[i] = (i == 0);
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
    // First half of tick cycle: tail first, then body+wings together.
    // Second half: body+wings reverse first, then tail reverse.
    const unsigned long now = millis();
    const unsigned long elapsed = now - patternStartTime;

    // Drive waves concurrently with the stingray animation (simple chases)
    {
        // Top wave: forward chase, ~150ms per step
        const int topCount = (int)(sizeof(TOP_WAVE_PINS)/sizeof(TOP_WAVE_PINS[0]));
        bool topStates[topCount];
        if (topCount > 0) {
            int topIndex = (int)((elapsed / 150UL) % (unsigned long)topCount);
            for (int i = 0; i < topCount; ++i) topStates[i] = (i == topIndex);
            if (topWave) topWave->runPattern(topStates, topCount);
        }

        // Bottom wave: reverse chase, ~200ms per step
        const int bottomCount = (int)(sizeof(BOTTOM_WAVE_PINS)/sizeof(BOTTOM_WAVE_PINS[0]));
        bool bottomStates[bottomCount];
        if (bottomCount > 0) {
            int bottomIndex = (int)((elapsed / 200UL) % (unsigned long)bottomCount);
            // reverse direction
            bottomIndex = (bottomCount - 1 - bottomIndex);
            for (int i = 0; i < bottomCount; ++i) bottomStates[i] = (i == bottomIndex);
            if (bottomWave) bottomWave->runPattern(bottomStates, bottomCount);
        }
    }

    const unsigned long cycleMs = (unsigned long)tickMax * (unsigned long)(delayTime > 0 ? delayTime : 1);
    if (cycleMs == 0) return;
    const unsigned long halfMs = cycleMs / 2;
    const unsigned long inCycle = elapsed % cycleMs;
    const bool firstHalf = inCycle < halfMs;
    const unsigned long posInHalf = firstHalf ? inCycle : (inCycle - halfMs);

    // Gather wing lines bottom->top
    LEDLine* lrh = stingRay.getLeftWingLEDLine("rhombus");
    LEDLine* lci = stingRay.getLeftWingLEDLine("circular");
    LEDLine* llo = stingRay.getLeftWingLEDLine("long");
    LEDLine* lsh = stingRay.getLeftWingLEDLine("short");
    LEDLine* ltr = stingRay.getLeftWingLEDLine("triangle");
    LEDLine* ltp = stingRay.getLeftWingLEDLine("top");
    LEDLine* linesL[6] = {lrh, lci, llo, lsh, ltr, ltp};

    LEDLine* rrh = stingRay.getRightWingLEDLine("rhombus");
    LEDLine* rci = stingRay.getRightWingLEDLine("circular");
    LEDLine* rlo = stingRay.getRightWingLEDLine("long");
    LEDLine* rsh = stingRay.getRightWingLEDLine("short");
    LEDLine* rtr = stingRay.getRightWingLEDLine("triangle");
    LEDLine* rtp = stingRay.getRightWingLEDLine("top");
    LEDLine* linesR[6] = {rrh, rci, rlo, rsh, rtr, rtp};

    // Tail and body
    LEDLine* tailLed = static_cast<LEDLine*>(stingRay.tail);
    LEDLine* headLed = static_cast<LEDLine*>(stingRay.head);
    RGBLine* body = stingRay.getBodyRGBLine("body");
    // Maintain oceanic color during animation (does not affect relays)
    if (body) {
        body->setRGBColor(0, 200, 255);
    }

    // Ensure left wing meta
    if (leftWingLengths[0] == 0 && leftWingLengths[1] == 0) computeLeftWingMeta();

    // compute physical counts for wings
    uint16_t leftTotals[6];
    uint16_t rightTotals[6];
    uint32_t leftSum = 0, rightSum = 0;
    for (int i = 0; i < 6; ++i) {
        leftTotals[i] = linesL[i] ? linesL[i]->getPhysicalLength() : 0;
        rightTotals[i] = linesR[i] ? linesR[i]->getPhysicalLength() : 0;
        leftSum += leftTotals[i];
        rightSum += rightTotals[i];
    }

    const uint16_t tailLen = tailLed ? tailLed->getPhysicalLength() : 0;
    const uint16_t headLen = headLed ? headLed->getPhysicalLength() : 0;
    const uint16_t bodyLen = BODY_RELAY_COUNT;

    // body+wing timeline length: choose max so body and wings finish together
    uint32_t wingsMax = leftSum > rightSum ? leftSum : rightSum;
    uint32_t bodyWingFrames = wingsMax > bodyLen ? wingsMax : bodyLen;
    if (bodyWingFrames == 0 && tailLen == 0) return;

    // total frames per half
    const uint32_t totalFrames = (uint32_t)tailLen + bodyWingFrames;
    if (totalFrames == 0) return;

    // map posInHalf -> frame index [0 .. totalFrames-1]
    uint32_t frame = (uint32_t)((posInHalf * totalFrames) / (halfMs ? halfMs : 1));
    if (frame >= totalFrames) frame = totalFrames - 1;

    // Helper lambda to compute a toDraw count (0..lineCount) given progressFrame in [0..bodyWingFrames-1]
    auto mapProgress = [](uint32_t progressFrame, uint32_t totalProgressFrames, uint32_t lineCount) -> uint16_t {
        if (lineCount == 0) return 0;
        if (progressFrame >= totalProgressFrames) return (uint16_t)lineCount;
        // add +1 to make first visible step light at least one unit once progress > 0
        uint32_t numer = (uint32_t)(progressFrame + 1) * (uint32_t)lineCount;
        uint16_t val = (uint16_t)(numer / totalProgressFrames);
        if (val == 0 && progressFrame > 0) val = 1;
        if (val > lineCount) val = (uint16_t)lineCount;
        return val;
    };

    // FIRST HALF
    if (firstHalf) {
        // If we're still in the tail-only phase:
        if (frame < tailLen) {
            // tail progressively lights from its start -> end (tail -> body)
            const uint16_t toDrawTail = (uint16_t)(frame + 1); // 1..tailLen
            if (tailLed) {
                // Use tail gradient (reuse top gradient for color)
                const uint8_t tailStart[3] = {0, 0, 48};
                const uint8_t tailEnd[3]   = {0, 255, 255};
                tailLed->runGradientProgress(tailStart, tailEnd, toDrawTail);
            }
            // head progressively lights from its start -> end
            if (headLed && headLen > 0) {
                const uint16_t toDrawHead = (uint16_t)(frame + 1); // 1..headLen (capped inside)
                const uint8_t headStart[3] = {0, 0, 48};
                const uint8_t headEnd[3]   = {0, 255, 255};
                headLed->runGradientProgress(headStart, headEnd, toDrawHead);
            }
            // body and wings remain off
            if (body) {
                for (int i = 0; i < BODY_RELAY_COUNT; ++i) body->updateRelay(i, false);
                body->apply();
            }
            for (int i = 0; i < 6; ++i) {
                if (linesL[i]) linesL[i]->clear();
                if (linesR[i]) linesR[i]->clear();
            }
            return;
        }

        // Tail finished; now body+wings animate simultaneously
        const uint32_t bodyProgressFrame = frame - tailLen; // 0 .. bodyWingFrames-1
        // Body: bottom -> top
        if (body) {
            const uint16_t toDrawBody = mapProgress(bodyProgressFrame, bodyWingFrames, bodyLen);
            for (uint16_t r = 0; r < BODY_RELAY_COUNT; ++r) {
                const bool on = (r < toDrawBody);
                body->updateRelay(r, on);
            }
            body->apply();
        }

        // Wings: for each line compute its toDraw and apply runGradientProgress (start->end)
        for (int i = 0; i < 6; ++i) {
            if (linesL[i]) {
                const uint16_t toDrawL = mapProgress(bodyProgressFrame, bodyWingFrames, leftTotals[i]);
                linesL[i]->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, toDrawL);
            }
            if (linesR[i]) {
                const uint16_t toDrawR = mapProgress(bodyProgressFrame, bodyWingFrames, rightTotals[i]);
                linesR[i]->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, toDrawR);
            }
        }

    } else {
        // SECOND HALF: reverse order
        // frame still mapped 0..totalFrames-1 where 0 means start of second half
        // First: reverse body+wings (top->bottom and wing end->start) while frame < bodyWingFrames
        if (frame < bodyWingFrames) {
            const uint32_t revProgress = frame; // 0..bodyWingFrames-1
            // Body: top -> bottom turn off (compute remaining)
            if (body) {
                // compute number of relays remaining lit
                uint16_t turnedOff = (uint16_t)((revProgress * bodyLen) / bodyWingFrames);
                if (turnedOff > bodyLen) turnedOff = bodyLen;
                uint16_t remaining = (turnedOff >= bodyLen) ? 0 : (bodyLen - turnedOff);
                // We need to light remaining from bottom->top, i.e., keep first 'remaining' relays ON
                for (uint16_t r = 0; r < BODY_RELAY_COUNT; r++) {
                    const bool on = (r < remaining);
                    body->updateRelay(r, on);
                }
                body->apply();
            }

            // Wings: for each line compute remaining = lineCount - turnedOff_for_line
            for (int i = 0; i < 6; ++i) {
                if (linesL[i]) {
                    uint16_t lineCount = leftTotals[i];
                    uint16_t turnedOffL = (uint16_t)((revProgress * lineCount) / bodyWingFrames);
                    if (turnedOffL > lineCount) turnedOffL = lineCount;
                    uint16_t remainingL = (turnedOffL >= lineCount) ? 0 : (lineCount - turnedOffL);
                    // draw 'remainingL' from start (which results in end->start being turned off progressively)
                    linesL[i]->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, remainingL);
                }
                if (linesR[i]) {
                    uint16_t lineCount = rightTotals[i];
                    uint16_t turnedOffR = (uint16_t)((revProgress * lineCount) / bodyWingFrames);
                    if (turnedOffR > lineCount) turnedOffR = lineCount;
                    uint16_t remainingR = (turnedOffR >= lineCount) ? 0 : (lineCount - turnedOffR);
                    linesR[i]->runGradientProgress(leftWingGradients[i].start, leftWingGradients[i].end, remainingR);
                }
            }

            return;
        }

        // After body+wings reversed, handle tail reverse.
        const uint32_t tailFrame = frame - bodyWingFrames; // 0..tailLen-1
        if (tailLed) {
            // For tail reverse we want to turn off from body -> tail.
            // We compute remaining units on tail (remaining = tailLen - (tailFrame+1))
            uint16_t turnedOffTail = (uint16_t)(((tailFrame) * tailLen) / (tailLen ? tailLen : 1));
            if (turnedOffTail > tailLen) turnedOffTail = tailLen;
            uint16_t remainingTail = (turnedOffTail >= tailLen) ? 0 : (tailLen - turnedOffTail);
            // draw remainingTail starting from the tail side (implementation assumes tailLed start==tail side)
            const uint8_t tailStart[3] = {0, 0, 48};
            const uint8_t tailEnd[3]   = {0, 255, 255};
            tailLed->runGradientProgress(tailStart, tailEnd, remainingTail);
        }
        if (headLed) {
            // Reverse head similarly: turn off from body -> head tip
            uint16_t turnedOffHead = (uint16_t)(((tailFrame) * headLen) / (headLen ? headLen : 1));
            if (turnedOffHead > headLen) turnedOffHead = headLen;
            uint16_t remainingHead = (turnedOffHead >= headLen) ? 0 : (headLen - turnedOffHead);
            const uint8_t headStart[3] = {0, 0, 48};
            const uint8_t headEnd[3]   = {0, 255, 255};
            headLed->runGradientProgress(headStart, headEnd, remainingHead);
        }

        // keep body and wings fully off when tail is finishing reverse
        if (frame >= bodyWingFrames) {
            if (body) {
                for (int i = 0; i < BODY_RELAY_COUNT; ++i) body->updateRelay(i, false);
                body->apply();
            }
            for (int i = 0; i < 6; ++i) {
                if (linesL[i]) linesL[i]->clear();
                if (linesR[i]) linesR[i]->clear();
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
        // Always-on: set white color on RGB pins
        body->setRGBColor(255, 255, 255);
    }

    // Keep LEDs on wings & tail solid white
    uint8_t color[1][3] = {{0, 0, 255}};

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
    if (auto headLed = static_cast<LEDLine*>(stingRay.head)) headLed->runPattern(color, 1);

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
