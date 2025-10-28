#include <Arduino.h>
#include "Turf.h"

Turf turf;

void setup()
{
  Serial.begin(9600);
  Serial.println("Main: setup start");
  turf.begin(); // ensure Turf init runs after Serial is ready
  Serial.println("Main: Turf begin() called");
}

void loop()
{
  static unsigned long lastTick = 0;
  if (millis() - lastTick > 75) {
    // Serial.println("Main: calling onTick");
    turf.onTick();
    lastTick = millis();
  }

  turf.loop2();
}
