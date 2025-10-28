#ifndef SEGMENT_H
#define SEGMENT_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LEDline;

class Segment
{
public:
    Segment();
    Segment(LEDline *parent, int start, int end, int virtualLength = 0, int offset = 0);

    void fill(uint32_t color);
    void clear();
    void comet(uint32_t tailColor, uint32_t headColor, uint8_t size, long tick, bool reverse = false);

    int getStart() const { return startIndex; }
    int getEnd() const { return endIndex; }
    int getLength() const { return endIndex - startIndex + 1; }
    int getVirtualLength() const { return (virtualLength > 0) ? virtualLength : getLength(); }
    int getOffset() const { return offset; }

private:
    LEDline *parent;
    int startIndex;
    int endIndex;
    int virtualLength;
    int offset;
};

#endif
