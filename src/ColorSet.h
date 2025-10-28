#pragma once
#include <Arduino.h>

#define MAX_COLORS 20

class ColorSet {
public:
    // ColorSet();
    ColorSet(uint8_t colors[][3], uint8_t colorCount);

    void makeGradient(uint8_t gradient[][3], uint8_t gradientCount) const;

private:
    uint8_t count;
    uint8_t r[MAX_COLORS];
    uint8_t g[MAX_COLORS];
    uint8_t b[MAX_COLORS];
};
