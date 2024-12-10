#include "pinconfig.hpp"
#include "snowflake.hpp"
#include <Arduino.h>

uint8_t last_animation = 0;
uint8_t animation_i = 1;
void setup()
{
  Snowflake::init();
  Snowflake::clear();
  Snowflake::bitShiftPowerOff();
}

void loop()
{

  if (!digitalRead(PIR_PIN))
  {
    Snowflake::bitShiftPowerOn();
    while (animation_i == last_animation)
      animation_i = random(5);

    last_animation = animation_i;
    switch (animation_i)
    {
    case 0:
      Snowflake::circularAnimation(2);
      break;
    case 1:
      Snowflake::circularAnimation(3);
      break;
    case 2:
      Snowflake::circularAnimation(5);
      break;
    case 3:
      Snowflake::outwardsAnimation();
      break;
    case 4:
      Snowflake::spinningAnimation();
      break;
    }

    Snowflake::clear();
  }
  Snowflake::bitShiftPowerOff();
  Snowflake::sleep();
}

ISR(PORTB_PORT_vect)
{
  cli();                          // disable global interrupts
  PORTB.PIN1CTRL &= ~PORT_ISC_gm; // disable interrupt.
  VPORTB.INTFLAGS |= true << 1;   // clear interrupt flag on pin PB1
}