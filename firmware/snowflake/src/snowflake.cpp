#include "snowflake.hpp"
#include "pinconfig.hpp"
#include <Arduino.h>

void Snowflake::init() {
  pinMode(STORE_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);
  pinMode(OUT_EN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(STORE_PIN, LOW);
  digitalWrite(CLEAR_PIN, LOW);
  digitalWrite(OUT_EN_PIN, HIGH);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);

  // PIR
  pinMode(PIR_PIN, INPUT_PULLUP);

  // ADC off by default
  pinMode(ADC_PIN, INPUT);
  pinMode(ADC_EN_PIN, INPUT);

  // Capacitive touch
  pinMode(TOUCH_PIN, INPUT);

  // Not used / free pins
  pinMode(TP1_PIN, INPUT);
  pinMode(TP2_PIN, INPUT);
  pinMode(TP3_PIN, INPUT);
  pinMode(TP4_PIN, INPUT);
  pinMode(TP5_PIN, INPUT);
  pinMode(TP6_PIN, INPUT);
  pinMode(TX_PIN, INPUT);
  pinMode(RX_PIN, INPUT);
}

void Snowflake::circularAnimation(uint8_t divider) {
  static uint8_t counter = 0;
  counter++;
  counter = counter > divider ? 1 : counter;
  _shiftSingle(counter == 1);
}

void Snowflake::randomAnimation() {
  uint8_t data[3] = {0, 0, 0};
  data[0] = random(256);
  data[1] = random(256);
  data[2] = random(256);
  _shiftAll(data);
}

void Snowflake::light(bool on) { digitalWrite(OUT_EN_PIN, !on); }

void Snowflake::clear() {
  digitalWrite(CLEAR_PIN, LOW);
  digitalWrite(CLEAR_PIN, HIGH);
  _shiftSingle(false);
}

void Snowflake::_shiftSingle(bool data) {
  digitalWrite(STORE_PIN, LOW);
  digitalWrite(DATA_PIN, data);
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(STORE_PIN, HIGH);
}

void Snowflake::_shiftAll(uint8_t *data) {
  digitalWrite(STORE_PIN, LOW);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data[0]);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data[1]);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data[2]);
  digitalWrite(STORE_PIN, HIGH);
}
