#include <Arduino.h>
#include "LEDline.h"
// LEDline line1(6, 46);

LEDline line1(11, 150);
LEDline line2(13, 125);
LEDline line3(9, 125);
LEDline line4(7, 125);
LEDline line5(5, 125);
LEDline line6(3, 125);
// LEDline line2(7, 130);

long pulse = 0;
float angle = 0.0;
float angleStep = 0.05;

// Colors
uint32_t RED = LEDline::color(255, 0, 0);
uint32_t GREEN = LEDline::color(0, 255, 0);
uint32_t BLUE = LEDline::color(0, 0, 255);
uint32_t YELLOW = LEDline::color(255, 255, 0);
uint32_t CYAN = LEDline::color(0, 255, 255);
uint32_t WHITE = LEDline::color(255, 255, 255);
uint32_t GOLD = LEDline::color(255, 215, 0);
uint32_t PURPLE = LEDline::color(128, 0, 128);

void setup()
{
  // Serial.begin(9600);

  // line1.addsegment(0, 39);
  line1.begin();
  line1.addsegment(0, 29);
  line1.addsegment(29, 29);
  line1.addsegment(58, 44);
  line1.addsegment(102, 23);

  line2.begin();
  line3.begin();
  line4.begin();
  line5.begin();
  line6.begin();

  for (int i = 0; i < 4; i++)
  {
    line1
  }
}

int com_len = 5;
void loop()
{

  // line1.commet(0, RED, BLUE, com_len, pulse, false, 50);
  line1.commet(1, RED, BLUE, com_len, pulse, false, 50);
  line1.commet(2, RED, BLUE, com_len, pulse, false, 50);
  line1.commet(3, RED, BLUE, com_len, pulse, false, 50);
  line1.commet(4, RED, BLUE, com_len, pulse, false, 50);

  // line2.commet(0, RED, BLUE, com_len, pulse, false, 100);
  // line3.commet(0, RED, BLUE, com_len, pulse, false, 100);
  // line4.commet(0, RED, BLUE, com_len, pulse, false, 100);
  // line5.commet(0, RED, BLUE, com_len, pulse, false, 100);
  // line6.commet(0, RED, BLUE, com_len, pulse, false, 100);
  // line1.commet(1, RED, BLUE, com_len, pulse, true, 58);
  // line1.commet(2, RED, BLUE, com_len, pulse, false, 21);

  // line1.commet(3, RED, BLUE, com_len, pulse, false, 21);
  // line1.commet(4, RED, BLUE, com_len, pulse, false, 21);
  // line1.commet(5, RED, BLUE, com_len, pulse, false, 21);

  // line2.commet(0, RED, BLUE, 10, pulse, false, 58);
  // line2.commet(1, RED, BLUE, 10, pulse + 42, true, 58);
  // line2.commet(2, RED, BLUE, 10, pulse + 49, false, 58);
  // line2.commet(3, RED, BLUE, 10, pulse + 35, true, 58);
  // line2.commet(4, RED, BLUE, 10, pulse + 35, false, 58);

  delay(2);
  pulse++;
}
