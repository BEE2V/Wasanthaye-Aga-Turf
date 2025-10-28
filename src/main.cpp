#include <Arduino.h>
#include "LEDline.h"
// LEDline line1(6, 46);

LEDline line1(3, 150);
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

  // pinMode(5, OUTPUT);    // Pin for the matrix
  // digitalWrite(5, HIGH); // Set the pin to HIGH (255) to power the matrix
  // matrix.addPingPoint(4, 7);
  // matrix.addPingPoint(6, 2);
  // matrix.addPingPoint(1, 5);
  // matrix.addPingPoint(2,3);
}

int com_len = 5;
void loop()
{

  line1.commet(0, RED, BLUE, com_len, pulse, false, 150);
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

  delay(10);
  pulse++;
}
