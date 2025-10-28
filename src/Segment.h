#pragma once
#include <Adafruit_NeoPixel.h>
#include "ColorSet.h"

class Segment {
    uint16_t ledCount;
    uint16_t startIndex;
    Adafruit_NeoPixel* strip;

public:
    explicit Segment(uint16_t count = 0, uint16_t start = 0);

    uint16_t getCount() const;

    void attachStrip(Adafruit_NeoPixel* neopixelStrip);

    void runPattern(const ColorSet* colorSet, uint16_t patternStartIndex = 0, uint16_t count = 0) const;
    void runPattern(uint8_t colors[][3], uint16_t colorCount, uint16_t patternStartIndex = 0, uint16_t count = 0) const;

    void clear() const;

    // True if this segment is attached to a physical LED strip
    bool hasStrip() const;

    // No-show variants to allow batching updates per line
    void runPatternNoShow(const ColorSet* colorSet, uint16_t patternStartIndex = 0, uint16_t count = 0) const;
    void runPatternNoShow(uint8_t colors[][3], uint16_t colorCount, uint16_t patternStartIndex = 0, uint16_t count = 0) const;
    void clearNoShow() const;
};
