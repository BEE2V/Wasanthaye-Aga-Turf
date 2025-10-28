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
  if (length <= 0) return;

  if (virtualLength <= 0) virtualLength = length;

  // normalize pulse to non-negative modulo
  int virtualIndex = (int)((pulse % virtualLength + virtualLength) % virtualLength);

  // Clear physical LEDs in segment
  for (int i = start; i <= end; ++i) {
    strip.setPixelColor(i, 0);
  }

  if (virtualIndex < length) {
    int index = reverse ? (end - virtualIndex) : (start + virtualIndex);
    strip.setPixelColor(index, color);
  }

  strip.show();
}

void LEDline::commet(int segment, uint32_t tailColor, uint32_t headColor, uint8_t size, long pulse, bool reverse, int virtualLength) {
  if (segment < 0 || segment >= segmentCount || size == 0) return;

  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int physicalLength = end - start + 1;
  if (physicalLength <= 0) return;

  if (virtualLength <= 0) virtualLength = physicalLength;

  // Clear all LEDs in the segment
  for (int i = start; i <= end; ++i) {
    strip.setPixelColor(i, 0);
  }

  // Head position in the virtual ring (normalized)
  int virtualHead = (int)((pulse % virtualLength + virtualLength) % virtualLength);

  // Precompute color components and deltas (use signed for deltas)
  uint8_t hr = (tailColor >> 16) & 0xFF;
  uint8_t hg = (tailColor >> 8) & 0xFF;
  uint8_t hb = tailColor & 0xFF;
  uint8_t tr = (headColor >> 16) & 0xFF;
  uint8_t tg = (headColor >> 8) & 0xFF;
  uint8_t tb = headColor & 0xFF;

  int dr = (int)tr - (int)hr;
  int dg = (int)tg - (int)hg;
  int db = (int)tb - (int)hb;

  // If size == 1, just draw the head color
  if (size == 1) {
    int vIndex = virtualHead % virtualLength;
    int physicalIndex = reverse ? end - vIndex : start + vIndex;
    if (physicalIndex >= start && physicalIndex <= end) {
      strip.setPixelColor(physicalIndex, Adafruit_NeoPixel::Color(tr, tg, tb));
    }
    strip.show();
    return;
  }

  int denom = int(size - 1);

  for (int i = 0; i < size; ++i) {
    int vIndex = virtualHead - i;
    // normalize vIndex into [0, virtualLength-1]
    vIndex = (vIndex % virtualLength + virtualLength) % virtualLength;

    int physicalIndex = reverse ? (end - vIndex) : (start + vIndex);

    if (physicalIndex < start || physicalIndex > end) continue;

    // Integer linear interpolation without floats:
    int r = (int(hr) * (denom - i) + int(tr) * i) / denom;
    int g = (int(hg) * (denom - i) + int(tg) * i) / denom;
    int b = (int(hb) * (denom - i) + int(tb) * i) / denom;

    strip.setPixelColor(physicalIndex, Adafruit_NeoPixel::Color((uint8_t)r, (uint8_t)g, (uint8_t)b));
  }

  strip.show();
}

// New helper: wheel color generator (0..255 -> RGB)
static uint32_t wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    return Adafruit_NeoPixel::Color(255 - pos * 3, 0, pos * 3);
  } else if (pos < 170) {
    pos -= 85;
    return Adafruit_NeoPixel::Color(0, pos * 3, 255 - pos * 3);
  } else {
    pos -= 170;
    return Adafruit_NeoPixel::Color(pos * 3, 255 - pos * 3, 0);
  }
}

// Fill entire segment with a single color (shows at end)
void LEDline::fillSegment(int segment, uint32_t color) {
  if (segment < 0 || segment >= segmentCount) return;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  if (end < start) return;
  for (int i = start; i <= end; ++i) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// Set global brightness (wraps Adafruit method and updates strip)
void LEDline::setBrightness(uint8_t brightness) {
  strip.setBrightness(brightness);
  strip.show();
}

// Circular rotate segment by steps. direction==true => rotate right (toward end)
void LEDline::rotateSegment(int segment, int steps, bool direction) {
  if (segment < 0 || segment >= segmentCount) return;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  if (len <= 0) return;

  // normalize steps
  steps = steps % len;
  if (steps < 0) steps += len;
  if (steps == 0) return;

  if (!direction) { // left rotation convert to equivalent right rotation
    steps = (len - steps) % len;
  }

  uint32_t* buf = new uint32_t[len];
  for (int i = 0; i < len; ++i) {
    buf[(i + steps) % len] = strip.getPixelColor(start + i);
  }
  for (int i = 0; i < len; ++i) {
    strip.setPixelColor(start + i, buf[i]);
  }
  delete[] buf;
  strip.show();
}

// Non-circular shift: positive steps shift toward end (right), negative toward start (left).
// Vacated pixels are filled with fillColor.
void LEDline::shiftSegment(int segment, int steps, uint32_t fillColor) {
  if (segment < 0 || segment >= segmentCount) return;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  if (len <= 0) return;
  if (steps == 0) return;

  // clamp steps
  if (abs(steps) >= len) {
    // all become fillColor
    for (int i = start; i <= end; ++i) strip.setPixelColor(i, fillColor);
    strip.show();
    return;
  }

  if (steps > 0) {
    // shift right: move from end to start
    for (int i = len - 1; i >= 0; --i) {
      int src = i - steps;
      if (src >= 0) strip.setPixelColor(start + i, strip.getPixelColor(start + src));
      else strip.setPixelColor(start + i, fillColor);
    }
  } else {
    // shift left
    int s = -steps;
    for (int i = 0; i < len; ++i) {
      int src = i + s;
      if (src < len) strip.setPixelColor(start + i, strip.getPixelColor(start + src));
      else strip.setPixelColor(start + i, fillColor);
    }
  }
  strip.show();
}

// Fill segment with linear gradient from startColor (at segment start) to endColor (at segment end)
void LEDline::gradientSegment(int segment, uint32_t startColor, uint32_t endColor) {
  if (segment < 0 || segment >= segmentCount) return;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  if (len <= 0) return;

  uint8_t sr = (startColor >> 16) & 0xFF;
  uint8_t sg = (startColor >> 8) & 0xFF;
  uint8_t sb = startColor & 0xFF;
  uint8_t er = (endColor >> 16) & 0xFF;
  uint8_t eg = (endColor >> 8) & 0xFF;
  uint8_t eb = endColor & 0xFF;

  if (len == 1) {
    strip.setPixelColor(start, Adafruit_NeoPixel::Color(er, eg, eb));
    strip.show();
    return;
  }

  int denom = len - 1;
  for (int i = 0; i < len; ++i) {
    int r = (int(sr) * (denom - i) + int(er) * i) / denom;
    int g = (int(sg) * (denom - i) + int(eg) * i) / denom;
    int b = (int(sb) * (denom - i) + int(eb) * i) / denom;
    strip.setPixelColor(start + i, Adafruit_NeoPixel::Color((uint8_t)r, (uint8_t)g, (uint8_t)b));
  }
  strip.show();
}

// Fill segment with linear gradient across multiple colors.
// Now accepts pointer + count instead of std::vector.
void LEDline::multicolorLinearGradientSegment(int segment, const uint32_t* colors, int colorCount) {
  if (segment < 0 || segment >= segmentCount) return;
  if (colors == nullptr || colorCount <= 0) return;

  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  if (len <= 0) return;

  // Single color -> fast path
  if (colorCount == 1) {
    for (int i = start; i <= end; ++i) strip.setPixelColor(i, colors[0]);
    strip.show();
    return;
  }

  int spans = colorCount - 1;
  int denom = len - 1;
  if (denom == 0) {
    strip.setPixelColor(start, colors[colorCount - 1]);
    strip.show();
    return;
  }

  for (int i = 0; i < len; ++i) {
    long pos_numer = (long)i * spans;
    int span = (int)(pos_numer / denom);
    int local = (int)(pos_numer % denom);

    if (span >= spans) {
      span = spans - 1;
      local = denom;
    }

    uint32_t c1 = colors[span];
    uint32_t c2 = colors[span + 1];

    uint8_t r1 = (c1 >> 16) & 0xFF;
    uint8_t g1 = (c1 >> 8) & 0xFF;
    uint8_t b1 = c1 & 0xFF;
    uint8_t r2 = (c2 >> 16) & 0xFF;
    uint8_t g2 = (c2 >> 8) & 0xFF;
    uint8_t b2 = c2 & 0xFF;

    int inv = denom - local;
    int r = (int(r1) * inv + int(r2) * local) / denom;
    int g = (int(g1) * inv + int(g2) * local) / denom;
    int b = (int(b1) * inv + int(b2) * local) / denom;

    strip.setPixelColor(start + i, Adafruit_NeoPixel::Color((uint8_t)r, (uint8_t)g, (uint8_t)b));
  }

  strip.show();
}

// Write an array of absolute pixel colors into the given segment (pixels[0] -> segment start).
void LEDline::setSegmentPixels(int segment, const uint32_t* pixels, int n) {
  if (segment < 0 || segment >= segmentCount) return;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  if (len <= 0) return;
  if (pixels == nullptr || n <= 0) {
    // clear entire segment
    for (int i = start; i <= end; ++i) strip.setPixelColor(i, 0);
    strip.show();
    return;
  }
  int limit = (n < len) ? n : len;
  for (int i = 0; i < limit; ++i) {
    strip.setPixelColor(start + i, pixels[i]);
  }
  // If caller provided fewer pixels than segment length, clear rest
  for (int i = limit; i < len; ++i) {
    strip.setPixelColor(start + i, 0);
  }
  strip.show();
}

// Add simple helper to return strip-compatible color
uint32_t LEDline::color(uint8_t r, uint8_t g, uint8_t b) {
  return Adafruit_NeoPixel::Color(r, g, b);
}

// Return the length (number of LEDs) in a given segment, or -1 if invalid
int LEDline::getSegmentLength(int segment) {
  if (segment < 0 || segment >= segmentCount) return -1;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  return (len > 0) ? len : -1;
}

// Return current number of segments
int LEDline::getSegmentCount() {
  return segmentCount;
}

// Set a single pixel inside a segment by index (0-based) and optionally show immediately.
void LEDline::setSegmentPixel(int segment, int indexInSegment, uint32_t color, bool showNow) {
  if (segment < 0 || segment >= segmentCount) return;
  int start = segmentStarts[segment];
  int end = segmentEnds[segment];
  int len = end - start + 1;
  if (len <= 0) return;
  if (indexInSegment < 0 || indexInSegment >= len) return;
  strip.setPixelColor(start + indexInSegment, color);
  if (showNow) strip.show();
}

void LEDline::clear() {
  strip.clear();
  strip.show();
}
