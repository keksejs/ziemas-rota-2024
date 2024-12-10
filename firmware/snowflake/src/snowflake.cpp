#include "snowflake.hpp"
#include "pinconfig.hpp"
#include <Arduino.h>
#include <avr/sleep.h>

const Snowflake::LED Snowflake::outerCircle[18] = {G, H, I, J, K, L, M, N, O,
                                                   P, Q, R, S, T, U, V, W, X};
const Snowflake::LED Snowflake::innerCircle[6] = {A, B, C, D, E, F};
const Snowflake::LED Snowflake::branch[6][4] = {{A, X, G, H}, {B, I, J, K}, {C, L, M, N}, {D, O, P, Q}, {E, R, S, T}, {F, U, V, W}};

void Snowflake::init()
{
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

void Snowflake::testAnimation() {}

void Snowflake::outwardsAnimation()
{
  for (uint8_t c = 0; c < 2; c++)
    for (uint8_t j = 0; j < 2; j++)
      for (uint8_t i = 0; i < 3; i++)
      {
        uint32_t data = 0;
        if (i >= 0)
        {
          data += branch[0 + j][0];
          data += branch[2 + j][0];
          data += branch[4 + j][0];
        }
        if (i >= 1)
        {
          data += branch[0 + j][1];
          data += branch[2 + j][1];
          data += branch[4 + j][1];
          data += branch[0 + j][3];
          data += branch[2 + j][3];
          data += branch[4 + j][3];
        }
        if (i == 2)
        {
          data += branch[0 + j][2];
          data += branch[2 + j][2];
          data += branch[4 + j][2];
        }

        _shiftAll_u24_t(data);
        delay(150);
      }
}

void Snowflake::circularAnimation(uint8_t divider)
{
  uint8_t counter = 0;
  for (uint8_t i = 0; i < 30; i++)
  {
    counter++;
    counter = counter > divider ? 1 : counter;
    _shiftSingle(counter == 1);
    _fadeDelay(300);
  }
}

void Snowflake::randomAnimation()
{
  uint8_t data[3] = {0, 0, 0};
  for (uint8_t i = 0; i < 30; i++)
  {
    data[0] = random(256);
    data[1] = random(256);
    data[2] = random(256);
    _shiftAll(data);

    _fadeDelay(100);
  }
}

void Snowflake::blinkAnimation()
{
  static bool onstate = false;

  if (onstate)
  {
    uint8_t data[3] = {0, 0, 0};
    _shiftAll(data);
  }
  else
  {
    uint8_t data[3] = {255, 255, 255};
    _shiftAll(data);
  }
  onstate ^= true;
}

void Snowflake::spinningAnimation()
{
  for (uint8_t i = 0; i < 2; i++)
  {
    for (uint8_t i = 0; i < 19; i++)
    {
      uint32_t data = 0;
      if (i == 0)
        data = outerCircle[i];
      else if (i == 18)
        data = outerCircle[0] + outerCircle[17];
      else
      {
        data = outerCircle[i] + outerCircle[i - 1];
      }
      _shiftAll_u24_t(data);
      delay(30);
    }
    for (uint8_t i = 0; i < 6; i++)
    {
      uint32_t data = 0;
      if (i == 0)
        data = innerCircle[i] + outerCircle[0];
      else
      {
        data = innerCircle[i] + innerCircle[i - 1];
      }
      _shiftAll_u24_t(data);
      delay(30);
    }

    for (uint8_t i = 0; i < 6; i++)
    {
      uint32_t data = 0;
      if (i == 0)
        data = innerCircle[i] + innerCircle[5];
      else
      {
        data = innerCircle[i] + innerCircle[i - 1];
      }
      _shiftAll_u24_t(data);
      delay(30);
    }
  }
}

void Snowflake::light(bool on) { digitalWrite(OUT_EN_PIN, !on); }

void Snowflake::clear()
{
  digitalWrite(CLEAR_PIN, LOW);
  digitalWrite(CLEAR_PIN, HIGH);
  _shiftSingle(false);
}

void Snowflake::bitShiftPowerOff()
{
  pinMode(DATA_PIN, INPUT);
  pinMode(CLK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  digitalWrite(CLEAR_PIN, LOW);   // active low
  digitalWrite(OUT_EN_PIN, HIGH); // active low
  pinMode(STORE_PIN, INPUT);
}

void Snowflake::bitShiftPowerOn()
{
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);
  pinMode(OUT_EN_PIN, OUTPUT);
  pinMode(STORE_PIN, OUTPUT);

  digitalWrite(STORE_PIN, LOW); // active high
  digitalWrite(CLEAR_PIN, LOW); // active low
  delay(1);
  digitalWrite(CLEAR_PIN, HIGH);
  digitalWrite(STORE_PIN, HIGH); // active high
  digitalWrite(OUT_EN_PIN, LOW); // active low
}

void Snowflake::sleep()
{
  VPORTB.INTFLAGS |= true << 1; // clear interrupt flag on pin PB1
  // PB1 is not fully asynchronous :(
  // Will wake up "Only from BOTHEDGES or LEVEL
  // interrupt sense configurations"
  pinMode(PIR_PIN, INPUT_PULLUP);
  PORTB.PIN1CTRL |= PORT_ISC_BOTHEDGES_gc;
  sei(); // enable global interrupts

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
}

void Snowflake::_shiftSingle(bool data)
{
  digitalWrite(STORE_PIN, LOW);
  digitalWrite(DATA_PIN, data);
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(STORE_PIN, HIGH);
}

void Snowflake::_shiftAll(uint8_t *data)
{
  digitalWrite(STORE_PIN, LOW);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data[0]);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data[1]);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data[2]);
  digitalWrite(STORE_PIN, HIGH);
}

void Snowflake::_shiftAll_u24_t(uint32_t data)
{
  digitalWrite(STORE_PIN, LOW);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, (data >> 16) & 0xFF);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, (data >> 8) & 0xFF);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data & 0xFF);
  digitalWrite(STORE_PIN, HIGH);
}

void Snowflake::_fadeDelay(uint16_t ms)
{
  const uint16_t f = 100;
  const uint32_t T_micros = 1e6 / f;
  const uint16_t count = ms / (1000 / f);
  const uint32_t delta = T_micros / count;

  uint32_t onTime_micros = 0;
  for (int n = 0; n < count; n++)
  {
    digitalWrite(OUT_EN_PIN, HIGH);
    delayMicroseconds(T_micros - onTime_micros);
    digitalWrite(OUT_EN_PIN, LOW);
    delayMicroseconds(onTime_micros);
    onTime_micros += delta;
  }

  digitalWrite(OUT_EN_PIN, LOW); // active low
}