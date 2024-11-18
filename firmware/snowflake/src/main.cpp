#include "pinconfig.hpp"
#include "snowflake.hpp"
#include <Arduino.h>

void setup() {
  Snowflake::init();
  // Snowflake::clear();
  // Snowflake::light(true);
}

void loop() {
  Snowflake::bitShiftPowerOn();

  if (!digitalRead(PIR_PIN)) {

    // for (int i = 0; i < 10; i++) {
    //   // Snowflake::randomAnimation();
    //   // Snowflake::circularAnimation(3);
    //   Snowflake::blinkAnimation();
    //   delay(200);
    // }

    // Snowflake::clear();

    Snowflake::testAnimation();
  }
  // for (int i = 0; i < 300; i++) {
  //   digitalWrite(OUT_EN_PIN, HIGH);
  //   delay(1);
  //   digitalWrite(OUT_EN_PIN, !(i % 2));
  //   delay(1);
  // }
  Snowflake::bitShiftPowerOff();
  Snowflake::sleep();
}

ISR(PORTB_PORT_vect) {
  cli();                          // disable global interrupts
  PORTB.PIN1CTRL &= ~PORT_ISC_gm; // disable interrupt.
  VPORTB.INTFLAGS |= true << 1;   // clear interrupt flag on pin PB1
}