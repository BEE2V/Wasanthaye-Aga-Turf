#include "LEDline.h"

LEDline line1(11, 28 + 29 + 44 + 23);
// LEDline line1(13, 7 + 6 + 7 + 5 + 4 + 5 + 5); // Total LEDs
long tick = 0;

void setup()
{
  line1.begin();

  // ✅ Only lengths, start index auto-calculated
  line1.addSegment(28);
  line1.addSegment(29);
  line1.addSegment(44);
  line1.addSegment(23);

  // line1.addSegment(7);
  // line1.addSegment(6);
  // line1.addSegment(7);
  // line1.addSegment(5);
  // line1.addSegment(4);
  // line1.addSegment(5);
  // line1.addSegment(5);
}

void loop()
{
  uint32_t RED = LEDline::color(255, 0, 0);

  int total = line1.count();
  int active = (tick / 20) % total;

  for (int i = 0; i < total; i++)
  {
    if (i == active)
      line1.segment(i).fill(RED);
    else
      line1.segment(i).clear();
  }

  tick++;
  delay(50);
}
