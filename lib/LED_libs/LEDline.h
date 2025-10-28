#ifndef LEDLINE_H
#define LEDLINE_H

#include <Adafruit_NeoPixel.h>
#include "Segment.h"

#define MAX_SEGMENTS 8

class LEDline
{
public:
    LEDline(uint8_t pin, int numPixels);
    void begin();
    void show();
    void clearAll();

    static uint32_t color(uint8_t r, uint8_t g, uint8_t b);

    // Automatically assign start index
    Segment &addSegment(int length, int virtualLength = 0, int offset = 0);

    Segment &segment(int index);
    int count() const { return segmentCount; }
    Adafruit_NeoPixel &getStrip() { return strip; }

private:
    Adafruit_NeoPixel strip;
    Segment segments[MAX_SEGMENTS];
    int segmentCount;
};

#endif
