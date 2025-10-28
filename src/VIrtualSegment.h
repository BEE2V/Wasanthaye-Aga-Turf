#pragma once
#include "Segment.h"

class VirtualSegment : public Segment {
public:
    explicit VirtualSegment(uint16_t count = 0, uint16_t start = 0)
        : Segment(count, start) {}
    // No additional behavior; treated as spacing. Segment methods will no-op without an attached strip.
};
