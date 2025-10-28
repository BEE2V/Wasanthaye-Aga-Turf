#ifndef LEDLINE_H
#define LEDLINE_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define MAX_SEGMENTS 20

class LEDline {
private:
  Adafruit_NeoPixel strip;
  int segmentStarts[MAX_SEGMENTS];
  int segmentEnds[MAX_SEGMENTS];
  int segmentCount;

public:
  LEDline(uint8_t pin, uint16_t totalLength);

  void begin();
  void addsegment(int start, int length);
  void runpixel(int segment, uint32_t color, long pulse, bool reverse, int virtualLength);
  void commet(int segment, uint32_t tailColor, uint32_t headColor, uint8_t size, long pulse , bool reverse , int virtualLength);

  // New helper APIs (implemented in LEDline.cpp)
  static uint32_t color(uint8_t r, uint8_t g, uint8_t b);
  void fillSegment(int segment, uint32_t color);
  void setBrightness(uint8_t brightness);
  void rotateSegment(int segment, int steps, bool direction);
  void shiftSegment(int segment, int steps, uint32_t fillColor);
  void gradientSegment(int segment, uint32_t startColor, uint32_t endColor);
  void multicolorLinearGradientSegment(int segment, const uint32_t* colors, int colorCount);
  void rainbowSegment(int segment, int offset);

  // Pixel-level helpers used by patterns (pointer + count)
  void setSegmentPixels(int segment, const uint32_t* pixels, int n);
  void setSegmentPixel(int segment, int indexInSegment, uint32_t color, bool showNow);

  // Query helper
  int getSegmentLength(int segment);

  // Return how many segments currently exist (new)
  int getSegmentCount();

  void clear();
};

#endif
