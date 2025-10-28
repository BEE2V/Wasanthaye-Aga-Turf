#pragma once
#include <Adafruit_NeoPixel.h>
#include "LineBase.h"
#include "Segment.h"
#include "SegmentSet.h"

#define MAX_SEGMENTS 10

class LEDLine final : public LineBase {
    Adafruit_NeoPixel strip;
    Segment* segments[MAX_SEGMENTS];
    uint8_t segmentCount;
    uint16_t totalCount;
    uint16_t virtualCount;
    uint8_t pin;

public:
    LEDLine(uint8_t pinNum, uint16_t totalLEDs, const SegmentSet* segments, size_t segmentCount);
    void begin();

    uint16_t getTotalCount() const;
    uint16_t getVirtualCount() const;
    uint16_t getCount() const;
    void addSegment(uint16_t start, uint16_t count);
    void addVirtualSegment(uint16_t start, uint16_t count);
    void runPattern(const ColorSet* colors, uint16_t startIndex = 0, uint16_t count = 0) const;
    void runPattern(uint8_t colors[][3], uint16_t colorCount, uint16_t patternStartIndex = 0, uint16_t count = 0) const;
    void clear() ;

    // Helpers for progressive animations
    uint16_t getPhysicalLength() const; // counts only segments with real LEDs
    void runGradientProgress(const uint8_t startRGB[3], const uint8_t endRGB[3], uint16_t litCount) const;
    void clearNoShow() const;
    void show() const;
};
