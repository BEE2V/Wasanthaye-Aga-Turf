#include <Arduino.h>
#include "Turf.h"

Turf turf;

void setup()
{
  Serial.begin(115200);
  turf = Turf();
}

void loop()
{
  static unsigned long lastTick = 0;
  if (millis() - lastTick > 200) {
    turf.onTick();
    lastTick = millis();
  }

  turf.loop();
}
