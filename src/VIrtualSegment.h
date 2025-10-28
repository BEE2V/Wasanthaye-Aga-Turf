#pragma once
#include "Segment.h"

class VirtualSegment : public Segment {
public:
    explicit VirtualSegment(const uint16_t count = 0, const uint16_t start = 0)
        : Segment(count, start) {}

    static void runPattern(ColorSet* colors, uint16_t start = 0, uint16_t count = 0);
    static void clear();
};
