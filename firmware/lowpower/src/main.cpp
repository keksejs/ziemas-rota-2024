#include <Arduino.h>
#include <avr/sleep.h>
#define STORE_PIN 3
#define PIR_PIN 8 // PB1 pin
#define CLEAR_PIN 5
#define OUT_EN_PIN 2
#define TX_PIN 7    // UART tests already
#define RX_PIN 6    // UART tests already
#define CLEAR_PIN 5 // bad pogo
#define CLK_PIN 4
#define DATA_PIN 1
#define ADC_PIN 11
#define ADC_EN_PIN 9
#define TP1_PIN 12 // PC2
#define TP2_PIN 13 // PC3
#define TP3_PIN 14 // PA1
#define TP4_PIN 15 // PA2
#define TP5_PIN 16 // PA3
#define TOUCH_PIN 10
#define TP6_PIN 0 // PA4

// uint8_t led_states[3] = {0};
// led_states[i] = random(0xFF);

void powerDown()
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

void resetPins()
{
  for (int i = 0; i < 16; i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
}

void bitShiftPowerOff()
{
  pinMode(DATA_PIN, INPUT);
  pinMode(CLK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  digitalWrite(CLEAR_PIN, LOW);   // active low
  digitalWrite(OUT_EN_PIN, HIGH); // active low
  pinMode(STORE_PIN, INPUT);
}

void bitShiftPowerOn()
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

void setup()
{
  resetPins();
  bitShiftPowerOn();

  pinMode(PIR_PIN, INPUT_PULLUP);
}

void loop()
{
  bitShiftPowerOn();
  while (!digitalRead(PIR_PIN))
    for (uint8_t n = 0; n < 10; n++)
    {
      digitalWrite(STORE_PIN, LOW); // active high
      for (uint8_t i = 0; i < 3; i++)
        shiftOut(DATA_PIN, CLK_PIN, LSBFIRST, (random(0xFF) & random(0xFF)));
      digitalWrite(STORE_PIN, HIGH); // active high
      delay(100);
    }
  bitShiftPowerOff();
  // delay(500);
  powerDown();
}

ISR(PORTB_PORT_vect)
{
  cli();                          // disable global interrupts
  PORTB.PIN1CTRL &= ~PORT_ISC_gm; // disable interrupt.
  VPORTB.INTFLAGS |= true << 1;   // clear interrupt flag on pin PB1
}
