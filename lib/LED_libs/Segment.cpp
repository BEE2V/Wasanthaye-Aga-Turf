#include "Segment.h"
#include "LEDline.h"

Segment::Segment() : parent(nullptr), startIndex(0), endIndex(0), virtualLength(0), offset(0) {}

Segment::Segment(LEDline *p, int start, int end, int vlen, int offs)
    : parent(p), startIndex(start), endIndex(end), virtualLength(vlen), offset(offs) {}

// Fill segment safely
void Segment::fill(uint32_t color)
{
    if (!parent)
        return;
    auto &strip = parent->getStrip();
    int total = strip.numPixels();
    for (int i = startIndex; i <= endIndex && i < total; i++)
        strip.setPixelColor(i, color);
    parent->show();
}

// Clear segment safely
void Segment::clear()
{
    if (!parent)
        return;
    auto &strip = parent->getStrip();
    int total = strip.numPixels();
    for (int i = startIndex; i <= endIndex && i < total; i++)
        strip.setPixelColor(i, 0);
    parent->show();
}

// Comet effect
void Segment::comet(uint32_t tailColor, uint32_t headColor, uint8_t size, long tick, bool reverse)
{
    if (!parent)
        return;
    auto &strip = parent->getStrip();
    int virtLen = getVirtualLength();
    int pos = (tick + offset) % virtLen;

    // Clear segment first
    for (int i = startIndex; i <= endIndex && i < strip.numPixels(); i++)
        strip.setPixelColor(i, 0);

    for (int i = 0; i < size; i++)
    {
        int vIndex = pos - i;
        if (vIndex < 0)
            vIndex += virtLen;

        int pixelIndex = reverse ? endIndex - vIndex : startIndex + vIndex;

        if (pixelIndex < startIndex || pixelIndex > endIndex || pixelIndex >= strip.numPixels())
            continue;

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

        strip.setPixelColor(pixelIndex, strip.Color(r, g, b));
    }
    parent->show();
}
