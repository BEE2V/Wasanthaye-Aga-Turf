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
  static uint32_t color(uint8_t r, uint8_t g, uint8_t b);
  void clear();
};

#endif
