#include "LEDline.h"
/*
 LEDline line1(11, 28 + 29 + 44 + 23);
 LEDline line1(13, 7 + 6 + 7 + 5 + 4 + 5 + 5);
 LEDline line1(9, 27 + 24);
 LEDline line1(7, 30 + 30 + 27);
 LEDline line1(5, 55 + 68);
 LEDline line1(3, 14 + 14 + 13 + 12 + 36);

*/
/*
 LEDline line1(3, 30 + 31 + 41 + 24);
 LEDline line1(5, 8 + 4 + 5 + 4 + 4 + 5 + 4 + 20);
 LEDline line1(7, 30 + 29);
 LEDline line1(9, 34 + 25 + 27);
 LEDline line1(11, 56 + 68);
 LEDline line1(13, 12 + 10 + 12 + 11 + 41);
*/
LEDline line1(13, 12 + 10 + 12 + 11 + 41);

long tick = 0;

void setup()
{
  line1.begin();

  /*
  line1.addSegment(28);
  line1.addSegment(29);
  line1.addSegment(44);
  line1.addSegment(23);

  line1.addSegment(7);
  line1.addSegment(6);
  line1.addSegment(7);
  line1.addSegment(5);
  line1.addSegment(4);
  line1.addSegment(5);
  line1.addSegment(5);


  line1.addSegment(27);
  line1.addSegment(24);

  line1.addSegment(30);
  line1.addSegment(30);
  line1.addSegment(27);

  line1.addSegment(55);
  line1.addSegment(68);

  line1.addSegment(14);
  line1.addSegment(14);
  line1.addSegment(13);
  line1.addSegment(12);
  line1.addSegment(36);
*/
  /*
  line1.addSegment(30);
  line1.addSegment(31);
  line1.addSegment(41);
  line1.addSegment(24);

  line1.addSegment(8);
  line1.addSegment(4);
  line1.addSegment(5);
  line1.addSegment(4);
  line1.addSegment(4);
  line1.addSegment(5);
  line1.addSegment(4);
  line1.addSegment(20);

  line1.addSegment(30);
  line1.addSegment(29);

  line1.addSegment(34);
  line1.addSegment(25);
  line1.addSegment(27);

  line1.addSegment(56);
  line1.addSegment(68);

  line1.addSegment(12);
  line1.addSegment(10);
  line1.addSegment(12);
  line1.addSegment(11);
  line1.addSegment(41);
  */
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
