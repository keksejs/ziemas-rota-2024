#include "pinconfig.hpp"
#include "snowflake.hpp"
#include <Arduino.h>

void setup() {
  Snowflake::init();
  Snowflake::clear();
  Snowflake::light(true);
}

void loop() {

  if (!digitalRead(PIR_PIN)) {
    for (int i = 0; i < 5; i++) {
      Snowflake::randomAnimation();
      delay(50);
    }
    Snowflake::clear();
  }
  // for (int i = 0; i < 300; i++) {
  //   digitalWrite(OUT_EN_PIN, HIGH);
  //   delay(1);
  //   digitalWrite(OUT_EN_PIN, !(i % 2));
  //   delay(1);
  // }
}