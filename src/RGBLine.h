#ifndef RGBLINE_H
#define RGBLINE_H

#include "LineBase.h"

#define MAX_RELAYS 20

class RGBLine final : public LineBase {
    int r;
    int g;
    int b;
    int relays[MAX_RELAYS];
    bool relayStates[MAX_RELAYS];
    int relayCount;

public:
    RGBLine(int red, int green, int blue, const int* relayPins, int count);

    void setRGB(int red, int green, int blue);
    void updateRelay(int index, bool value);
    void updateRelays(const int* indices, const bool* values, int count);
    void apply() const; // Set all relay pins to their current state
    void clear();
};

#endif
