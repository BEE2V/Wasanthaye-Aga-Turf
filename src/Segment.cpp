#include "Segment.h"

Segment::Segment(const uint16_t count, const uint16_t start)
    : ledCount(count), startIndex(start), strip(nullptr) {}

uint16_t Segment::getCount() const { return ledCount; }

void Segment::attachStrip(Adafruit_NeoPixel* neopixelStrip) {
    this->strip = neopixelStrip;
}

void Segment::runPattern(const ColorSet* colorSet, const uint16_t patternStartIndex, const uint16_t count) const {
    if (!strip) return;

    uint8_t gradient[MAX_COLORS][3];
    colorSet->makeGradient(gradient, ledCount);

    const uint16_t limit = count == 0 || count > ledCount ? ledCount : count;

    for (uint16_t i = 0; i < limit; i++) {
        const uint16_t colorIndex = (patternStartIndex + i) % ledCount;
        const uint32_t color = strip->Color(gradient[colorIndex][0], gradient[colorIndex][1], gradient[colorIndex][2]);
        strip->setPixelColor(startIndex + i, color);
    }

    strip->show();
}

void Segment::runPattern(uint8_t colors[][3], const uint16_t colorCount, const uint16_t patternStartIndex, const uint16_t count) const {
    if (!strip || colorCount == 0) return;

    const uint16_t limit = count == 0 || count > ledCount ? ledCount : count;

    for (uint16_t i = 0; i < limit; i++) {
        const uint16_t colorIndex = (patternStartIndex + i) % colorCount;
        const uint32_t color = strip->Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
        strip->setPixelColor(startIndex + i, color);
    }

    strip->show();
}

void Segment::clear() const {
    if (!strip) return;

    // Prefer Adafruit_NeoPixel::fill(start, count) when available to clear only this segment.
    // Many Adafruit_NeoPixel versions provide: fill (color, first, count)
    // Use conditional compilation to avoid compile errors on older library versions.
#if defined(ADAFRUIT_NEOPIXEL_H)
    // clear only this segment (color 0 == black)
    strip->fill(0, startIndex, ledCount);
#else
    // Fallback: set each pixel in the segment to black
    for (uint16_t i = 0; i < ledCount; i++) {
        strip->setPixelColor(startIndex + i, 0);
    }
#endif

    strip->show();
}

bool Segment::hasStrip() const {
    return strip != nullptr;
}

void Segment::runPatternNoShow(const ColorSet* colorSet, const uint16_t patternStartIndex, const uint16_t count) const {
    if (!strip) return;

    uint8_t gradient[MAX_COLORS][3];
    colorSet->makeGradient(gradient, ledCount);

    const uint16_t limit = count == 0 || count > ledCount ? ledCount : count;

    for (uint16_t i = 0; i < limit; i++) {
        const uint16_t colorIndex = (patternStartIndex + i) % ledCount;
        const uint32_t color = strip->Color(gradient[colorIndex][0], gradient[colorIndex][1], gradient[colorIndex][2]);
        strip->setPixelColor(startIndex + i, color);
    }
}

void Segment::runPatternNoShow(uint8_t colors[][3], const uint16_t colorCount, const uint16_t patternStartIndex, const uint16_t count) const {
    if (!strip || colorCount == 0) return;

    const uint16_t limit = count == 0 || count > ledCount ? ledCount : count;

    for (uint16_t i = 0; i < limit; i++) {
        const uint16_t colorIndex = (patternStartIndex + i) % colorCount;
        const uint32_t color = strip->Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
        strip->setPixelColor(startIndex + i, color);
    }
}

void Segment::clearNoShow() const {
    if (!strip) return;
#if defined(ADAFRUIT_NEOPIXEL_H)
    strip->fill(0, startIndex, ledCount);
#else
    for (uint16_t i = 0; i < ledCount; i++) {
        strip->setPixelColor(startIndex + i, 0);
    }
#endif
}