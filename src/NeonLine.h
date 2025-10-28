#ifndef NEONLINE_H
#define NEONLINE_H
#include <stdint.h>

class NeonLine {
    uint8_t* pins;
    int pinCount;

public:
    NeonLine(uint8_t* pins, int pinCount);
    void runPattern(const bool* states, int stateCount) const;
    void clear() const;
};

#endif
