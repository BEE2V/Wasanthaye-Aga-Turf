#ifndef PATTERNONE_H
#define PATTERNONE_H

#include <Arduino.h>
#include "LEDline.h"

inline void runPatternOne() {
  // Simple pattern: report to Serial and blink the built-in LED once.
  Serial.println("patternOne: start");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  Serial.println("patternOne: done");
}

// Helper: scale a 24-bit color (0xRRGGBB) by intensity 0..255 (integer math)
static inline uint32_t colorScale(uint32_t color, uint8_t intensity) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  uint8_t rn = (uint8_t)((int(r) * intensity) / 255);
  uint8_t gn = (uint8_t)((int(g) * intensity) / 255);
  uint8_t bn = (uint8_t)((int(b) * intensity) / 255);
  return Adafruit_NeoPixel::Color(rn, gn, bn);
}

// Run a triangular-wave "moving peak" across a segment.
// - line: LEDline instance
// - segment: target segment index
// - baseColor: peak color (Adafruit_NeoPixel::Color(R,G,B))
// - waveWidth: half-width of the peak (controls spread); typical 3..8
// - iterations: number of frames to advance (use large for continuous)
// - delayMs: delay between frames in ms
inline void runWavePattern(LEDline& line, int segment, uint32_t baseColor, int waveWidth = 5, int iterations = 200, int delayMs = 40) {
  if (segment < 0) return;

  // Query segment length via LEDline getter; fallback to 60 if unavailable/invalid
  int segLen = line.getSegmentLength(segment);
  if (segLen <= 0) segLen = 60; // adjust to real strip length if known

  // allocate pixel buffer on the heap
  uint32_t* pixels = new uint32_t[segLen];

  int cycleLen = segLen + waveWidth * 2; // allow wave to move fully across edges
  int half = waveWidth > 0 ? waveWidth : 0;

  for (int frame = 0; frame < iterations; ++frame) {
    int phase = frame % cycleLen; // center position of the peak runs from 0..cycleLen-1
    int centerPos = phase - half; // center moves from -half .. segLen+half-1
    for (int p = 0; p < segLen; ++p) {
      int dist = abs(p - centerPos);
      int intensity = 0;
      if (waveWidth > 0 && dist <= waveWidth) {
        // triangular envelope: intensity proportional to (waveWidth - dist)
        intensity = (waveWidth - dist) * 255 / waveWidth;
        if (intensity < 0) intensity = 0;
        if (intensity > 255) intensity = 255;
      }
      pixels[p] = colorScale(baseColor, (uint8_t)intensity);
    }
    line.setSegmentPixels(segment, pixels, segLen);
    if (delayMs > 0) delay(delayMs);
  }

  delete[] pixels;
}

#endif // PATTERNONE_H
