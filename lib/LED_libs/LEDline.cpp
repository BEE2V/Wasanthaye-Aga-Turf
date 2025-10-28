#include "LEDline.h"
#include "Segment.h"

LEDline::LEDline(uint8_t pin, int numPixels)
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800), segmentCount(0) {}

void LEDline::begin()
{
    strip.begin();
    strip.show();
}

void LEDline::show()
{
    strip.show();
}

void LEDline::clearAll()
{
    strip.clear();
    strip.show();
}

uint32_t LEDline::color(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

// Add segment with auto start index
Segment &LEDline::addSegment(int length, int virtualLength, int offset)
{
    if (segmentCount >= MAX_SEGMENTS)
        return segments[MAX_SEGMENTS - 1];

    int start = 0;
    if (segmentCount > 0)
    {
        start = segments[segmentCount - 1].getEnd() + 1;
    }

    int end = start + length - 1;
    if (end >= strip.numPixels())
        end = strip.numPixels() - 1;

    segments[segmentCount] = Segment(this, start, end, virtualLength, offset);
    return segments[segmentCount++];
}

Segment &LEDline::segment(int index)
{
    if (index < 0)
        index = 0;
    if (index >= segmentCount)
        index = segmentCount - 1;
    return segments[index];
}
