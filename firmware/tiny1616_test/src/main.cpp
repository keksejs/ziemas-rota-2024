#include <Arduino.h>
#define STORE_PIN 3
#define PIR_PIN 8
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

const uint8_t pogo_pins[16] = {
    STORE_PIN,
    PIR_PIN,
    CLEAR_PIN,
    OUT_EN_PIN,
    TX_PIN,
    RX_PIN,
    CLEAR_PIN,
    CLK_PIN,
    DATA_PIN,
    ADC_PIN,
    ADC_EN_PIN,
    TP1_PIN,
    TP2_PIN,
    TP3_PIN,
    TP4_PIN,
    TP5_PIN};

void resetPins()
{
  for (int i = 0; i < 16; i++)
  {
    if (i != TX_PIN && i != RX_PIN)
    {
      pinMode(i, INPUT);
      digitalWrite(i, LOW);
    }
  }
}

void setup()
{
  resetPins();
  Serial.pins(TX_PIN, RX_PIN);
  Serial.begin(9600);
  // https://github.com/SpenceKonde/megaTinyCore/issues/323
}

void loop()
{
  if (Serial.available())
  {
    uint8_t msg = Serial.read();
    Serial.read();
    Serial.read();
    Serial.read();
    if (msg != 0 && msg < 17)
    {
      if (msg != RX_PIN && msg != TX_PIN)
      {
        pinMode(msg, OUTPUT);
        digitalWrite(msg, LOW);
        delay(50);
        pinMode(msg, INPUT);
        Serial.write(msg);
      }
    }
    else if (msg == 30)
    {
      Serial.write(msg);
    }
  }
}
