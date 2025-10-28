#include "LEDLine.h"
#include "VIrtualSegment.h"

LEDLine::LEDLine(const uint8_t pinNum, const uint16_t totalLEDs, const SegmentSet* segments, const size_t segmentCount)
    : strip(totalLEDs, pinNum, NEO_GRB + NEO_KHZ800),
      segmentCount(0),
      totalCount(0),
      pin(pinNum) {
    this->begin();
    for (size_t i = 0; i < segmentCount; i++) {
        const SegmentSet& segSet = segments[i];
        if (segSet.type == 0) {
            this->addSegment(totalCount, segSet.count);
        } else if (segSet.type == 1) {
            this->addVirtualSegment(totalCount, segSet.count);
        }
    }
}

void LEDLine::begin() {
    strip.begin();
    strip.show();
}

uint16_t LEDLine::getTotalCount() const {
    return totalCount;
}

uint16_t LEDLine::getVirtualCount() const {
    return virtualCount;
}

uint16_t LEDLine::getCount() const {
    return totalCount - virtualCount;
}

void LEDLine::addSegment(const uint16_t start, const uint16_t count) {
    if (segmentCount >= MAX_SEGMENTS) return;
    const auto segment = new Segment(count, start);
    segment->attachStrip(&strip);
    segments[segmentCount++] = segment;
    totalCount += count;
}

void LEDLine::addVirtualSegment(const uint16_t start, const uint16_t count) {
    if (segmentCount >= MAX_SEGMENTS) return;
    const auto segment = new VirtualSegment(count, start);
    segments[segmentCount++] = segment;
    totalCount += count;
}

void LEDLine::runPattern(const ColorSet* colors, uint16_t startIndex, uint16_t count) const {
    for (uint8_t i = 0; i < segmentCount; i++) {
        const Segment* seg = segments[i];
        if (startIndex >= seg->getCount()) {
            startIndex -= seg->getCount();
            continue;
        }

        const uint16_t modCount = count > seg->getCount() ? seg->getCount() : count;
        seg->runPattern(colors, 0, modCount);
        startIndex = 0;
        count -= modCount;
    }
}

void LEDLine::runPattern(uint8_t colors[][3], const uint16_t colorCount, uint16_t patternStartIndex, uint16_t count) const {
    for (uint8_t i = 0; i < segmentCount; i++) {
        const Segment* seg = segments[i];
        if (patternStartIndex >= seg->getCount()) {
            patternStartIndex -= seg->getCount();
            continue;
        }

        const uint16_t modCount = count > seg->getCount() ? seg->getCount() : count;
        seg->runPattern(colors, colorCount, 0, modCount);
        patternStartIndex = 0;
        count -= modCount;
    }
}

void LEDLine::clear() {
    strip.clear();
    strip.show();
}
