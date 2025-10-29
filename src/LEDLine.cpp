#include "LEDLine.h"
#include "VIrtualSegment.h"

LEDLine::LEDLine(const uint8_t pinNum, const uint16_t totalLEDs, const SegmentSet* segments, const size_t segmentCount)
    : strip(totalLEDs, pinNum, NEO_GRB + NEO_KHZ800),
      segmentCount(0),
      totalCount(0),
      virtualCount(0),
      pin(pinNum) {
    this->begin();
    for (size_t i = 0; i < segmentCount; i++) {
        const SegmentSet& segSet = segments[i];
        if (segSet.type == 0) {
            this->addSegment(totalCount, segSet.count);
        } else if (segSet.type == 1) {
            // Virtual segments do not occupy physical LED indices.
            this->addVirtualSegment(0, segSet.count);
            virtualCount += segSet.count;
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
    // Do not advance totalCount for virtual spacing
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

void LEDLine::clearNoShow() const {
    // Clear only physical segments without issuing show
    for (uint8_t i = 0; i < segmentCount; i++) {
        const Segment* seg = segments[i];
        if (!seg || !seg->hasStrip()) continue;
        const_cast<Segment*>(seg)->clearNoShow();
    }
}

void LEDLine::show() const {
    const_cast<Adafruit_NeoPixel&>(strip).show();
}

uint16_t LEDLine::getPhysicalLength() const {
    uint16_t length = 0;
    for (uint8_t i = 0; i < segmentCount; i++) {
        const Segment* seg = segments[i];
        // Only count segments attached to the strip (skip virtual)
        if (seg) {
            // rely on dynamic dispatch to check if it has strip: use Segment::hasStrip if available
            // Since we cannot RTTI here reliably, approximate: VirtualSegment never attaches strip in this codebase
            // We can attempt to draw a zero pixel and check? Not at runtime here.
            // Simpler: in this project VirtualSegment is created without attachStrip, so getCount()>0 but no strip.
            // We expose hasStrip() on Segment.
            if (seg->hasStrip()) {
                length += seg->getCount();
            }
        }
    }
    return length;
}

void LEDLine::runGradientProgress(const uint8_t startRGB[3], const uint8_t endRGB[3], uint16_t litCount) const {
    // Draw a per-segment progressive gradient up to litCount across only physical LEDs
    uint16_t remaining = litCount;
    for (uint8_t i = 0; i < segmentCount; i++) {
        const Segment* seg = segments[i];
        if (!seg || !seg->hasStrip()) continue;

        const uint16_t segLen = seg->getCount();
        if (remaining == 0) {
            // clear this physical segment
            const_cast<Segment*>(seg)->clearNoShow();
            continue;
        }

        const uint16_t draw = (remaining >= segLen) ? segLen : remaining;

        if (draw == 0) {
            const_cast<Segment*>(seg)->clearNoShow();
        } else {
            // Build a small gradient buffer from startRGB -> endRGB for 'draw' LEDs
            uint8_t colorsBuf[3][3]; // placeholder to satisfy signature when draw==1
            if (draw == 1) {
                colorsBuf[0][0] = endRGB[0];
                colorsBuf[0][1] = endRGB[1];
                colorsBuf[0][2] = endRGB[2];
                const_cast<Segment*>(seg)->runPatternNoShow(colorsBuf, 1, 0, 1);
                // Clear the rest of the segment
                if (segLen > 1) {
                    // Clear remaining part by drawing black over rest
                    uint8_t black[1][3] = {{0,0,0}};
                    const_cast<Segment*>(seg)->runPatternNoShow(black, 1, 1, segLen - 1);
                }
            } else {
                // Create a gradient array of size 'draw'
                const uint16_t maxLocal = draw;
                // Limit local buffer size: create on heap to avoid large stack on AVR
                uint8_t (*grad)[3] = new uint8_t[maxLocal][3];
                for (uint16_t j = 0; j < maxLocal; j++) {
                    const float t = (maxLocal <= 1) ? 1.0f : (float)j / (float)(maxLocal - 1);
                    grad[j][0] = (uint8_t)(startRGB[0] + (endRGB[0] - startRGB[0]) * t);
                    grad[j][1] = (uint8_t)(startRGB[1] + (endRGB[1] - startRGB[1]) * t);
                    grad[j][2] = (uint8_t)(startRGB[2] + (endRGB[2] - startRGB[2]) * t);
                }
                const_cast<Segment*>(seg)->runPatternNoShow(grad, maxLocal, 0, draw);
                delete[] grad;

                // Clear remainder of this segment if any
                if (segLen > draw) {
                    uint8_t black[1][3] = {{0,0,0}};
                    const_cast<Segment*>(seg)->runPatternNoShow(black, 1, draw, segLen - draw);
                }
            }
        }

        if (remaining >= segLen) remaining -= segLen; else remaining = 0;
    }
    // Single show after all segments updated
    const_cast<Adafruit_NeoPixel&>(strip).show();
}

// --- New methods implemented below ---

uint8_t LEDLine::getSegmentCount() const {
    return segmentCount;
}

int LEDLine::getSegmentLength(int index) const {
    if (index < 0 || index >= segmentCount) return 0;
    const Segment* seg = segments[index];
    return seg ? (int)seg->getCount() : 0;
}

void LEDLine::clearSegment(int index) const {
    if (index < 0 || index >= segmentCount) return;
    Segment* seg = segments[index];
    if (!seg) return;
    if (seg->hasStrip()) {
        seg->clear(); // this will call strip->show() internally
    } else {
        // Virtual segments: nothing to do
    }
}

void LEDLine::gradientSegment(int index, const uint8_t startRGB[3], const uint8_t endRGB[3], uint16_t drawCount) const {
    if (index < 0 || index >= segmentCount) return;
    const Segment* seg = segments[index];
    if (!seg) return;

    const uint16_t segLen = seg->getCount();
    if (drawCount == 0) {
        // clear entire segment
        if (seg->hasStrip()) const_cast<Segment*>(seg)->clear();
        return;
    }

    // Cap drawCount to segment length
    if (drawCount > segLen) drawCount = segLen;

    if (!seg->hasStrip()) return; // nothing to draw for virtual segments

    if (drawCount == segLen) {
        // full segment gradient
        // build gradient array of segLen
        uint8_t (*grad)[3] = new uint8_t[segLen][3];
        for (uint16_t j = 0; j < segLen; ++j) {
            const float t = (segLen <= 1) ? 1.0f : (float)j / (float)(segLen - 1);
            grad[j][0] = (uint8_t)(startRGB[0] + (endRGB[0] - startRGB[0]) * t);
            grad[j][1] = (uint8_t)(startRGB[1] + (endRGB[1] - startRGB[1]) * t);
            grad[j][2] = (uint8_t)(startRGB[2] + (endRGB[2] - startRGB[2]) * t);
        }
        const_cast<Segment*>(seg)->runPatternNoShow(grad, segLen, 0, segLen);
        delete[] grad;
    } else {
        // partial gradient for the first 'drawCount' pixels of this segment (start->end over drawCount)
        uint8_t (*grad)[3] = new uint8_t[drawCount][3];
        for (uint16_t j = 0; j < drawCount; ++j) {
            const float t = (drawCount <= 1) ? 1.0f : (float)j / (float)(drawCount - 1);
            grad[j][0] = (uint8_t)(startRGB[0] + (endRGB[0] - startRGB[0]) * t);
            grad[j][1] = (uint8_t)(startRGB[1] + (endRGB[1] - startRGB[1]) * t);
            grad[j][2] = (uint8_t)(startRGB[2] + (endRGB[2] - startRGB[2]) * t);
        }
        const_cast<Segment*>(seg)->runPatternNoShow(grad, drawCount, 0, drawCount);
        delete[] grad;

        // clear rest of segment
        if (segLen > drawCount) {
            uint8_t black[1][3] = {{0,0,0}};
            const_cast<Segment*>(seg)->runPatternNoShow(black, 1, drawCount, segLen - drawCount);
        }
    }

    // show after updating this segment
    const_cast<Adafruit_NeoPixel&>(strip).show();
}
