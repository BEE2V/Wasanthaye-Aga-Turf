#include "ColorSet.h"

// ColorSet::ColorSet() : count(0) {}

ColorSet::ColorSet(uint8_t colors[][3], const uint8_t colorCount) {
    count = colorCount > MAX_COLORS ? MAX_COLORS : colorCount;
    for (uint8_t i = 0; i < count; i++) {
        r[i] = colors[i][0];
        g[i] = colors[i][1];
        b[i] = colors[i][2];
    }
}

void ColorSet::makeGradient(uint8_t gradient[][3], const uint8_t gradientCount) const {
    if (gradientCount < count) return;
    for (uint8_t i = 0; i < gradientCount; i++) {
        const float t = static_cast<float>(i) / static_cast<float>(gradientCount - 1);
        gradient[i][0] = r[0] + (r[count - 1] - r[0]) * t;
        gradient[i][1] = g[0] + (g[count - 1] - g[0]) * t;
        gradient[i][2] = b[0] + (b[count - 1] - b[0]) * t;
    }
}
