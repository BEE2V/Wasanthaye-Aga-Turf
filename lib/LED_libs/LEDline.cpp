#include "LEDline.h"

LEDline::LEDline(uint8_t pin, uint16_t totalLength)
  : strip(totalLength, pin, NEO_GRB + NEO_KHZ800), segmentCount(0) {}

void LEDline::begin() {
  strip.begin();
  strip.show();
  segmentStarts[0] = 0;
  segmentEnds[0] = strip.numPixels() - 1;
  segmentCount = 1;
}

void LEDline::addsegment(int start, int length) {
  int end = start + length - 1;
  if (segmentCount >= MAX_SEGMENTS) return;
  if (start < 0 || end >= strip.numPixels()) return;

  segmentStarts[segmentCount] = start;
  segmentEnds[segmentCount] = end;
  segmentEnds[segmentCount - 1] = start - 1;
  segmentCount++;
}

void LEDline::runpixel(int segment, uint32_t color, long pulse, bool reverse, int virtualLength) {
  if (segment < 0 || segment >= segmentCount) return;

  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int length = end - start + 1;

  // Use virtual length for animation timing
  int virtualIndex = pulse % virtualLength;

  // Clear physical LEDs
  for (int i = start; i <= end; ++i) {
    strip.setPixelColor(i, 0);
  }

  // Only light if virtual index is inside physical range
  if (virtualIndex < length) {
    int index = reverse 
      ? end - virtualIndex 
      : start + virtualIndex;
    strip.setPixelColor(index, color);
  }

  strip.show();
}

void LEDline::commet(int segment, uint32_t tailColor, uint32_t headColor, uint8_t size, long pulse, bool reverse, int virtualLength) {
  if (segment < 0 || segment >= segmentCount || size == 0) return;

  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int physicalLength = end - start + 1;

  // Clear all LEDs in the segment
  for (int i = start; i <= end; ++i) {
    strip.setPixelColor(i, 0);
  }

  // Head position in the virtual ring
  int virtualHead = pulse % virtualLength;

  for (int i = 0; i < size; ++i) {
    int vIndex = (virtualHead - i + virtualLength) % virtualLength;

    int physicalIndex = reverse
      ? end - vIndex
      : start + vIndex;

    if (physicalIndex < start || physicalIndex > end) continue;

    float t = float(i) / float(size - 1);
    uint8_t hr = (tailColor >> 16) & 0xFF;
    uint8_t hg = (tailColor >> 8) & 0xFF;
    uint8_t hb = tailColor & 0xFF;
    uint8_t tr = (headColor >> 16) & 0xFF;
    uint8_t tg = (headColor >> 8) & 0xFF;
    uint8_t tb = headColor & 0xFF;

    uint8_t r = hr + (tr - hr) * t;
    uint8_t g = hg + (tg - hg) * t;
    uint8_t b = hb + (tb - hb) * t;

    strip.setPixelColor(physicalIndex, Adafruit_NeoPixel::Color(r, g, b));
  }

  strip.show();
}


uint32_t LEDline::color(uint8_t r, uint8_t g, uint8_t b) {
  return Adafruit_NeoPixel::Color(r, g, b);
}

void LEDline::clear() {
  strip.clear();
  strip.show();
}

