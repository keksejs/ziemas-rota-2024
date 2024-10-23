#include <Arduino.h>
#include <PicoSoftwareSerial.h>

SoftwareSerial attinySerial(5, 4); // TX ; RX

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

#define TOUCH_PIN 10
#define TP5_PIN 16 // PA3
#define TP6_PIN 0  // PA4

const uint8_t pogo_pins[15] = {
    STORE_PIN,
    PIR_PIN,
    CLEAR_PIN,
    OUT_EN_PIN,
    TX_PIN,
    RX_PIN,
    40,
    CLK_PIN,
    DATA_PIN,
    ADC_PIN,
    ADC_EN_PIN,
    TP1_PIN,
    TP2_PIN,
    TP3_PIN,
    TP4_PIN};

uint8_t receive()
{
  if (attinySerial.available())
    return attinySerial.read();

  return 0;
}

void readPins(uint8_t picoPin)
{
  bool pinStates[15] = {true};
  for (int i = 0; i < 16; i++)
    pinStates[i] = digitalRead(i);

  if (!pinStates[picoPin])
  {
    Serial.print("PIN ");
    Serial.print(picoPin);
    Serial.println(" GOOD");
  }
  else
  {
    Serial.print("!!! PIN ");
    Serial.print(picoPin);
    Serial.println(" NO CONTACT !!!");
  }

  // for (int i = 0; i < 16; i++)
  // {
  //   if (i == picoPin)
  //     break;
  //   if (!pinStates[i] && pogo_pins[i] != RX_PIN && pogo_pins[i] != TX_PIN)
  //   {
  //     Serial.print("!!! SHORT ON PIN ");
  //     Serial.println(i);
  //   }
  // }
}

void fullTest()
{
  Serial.println("\n\n\n\nTEST START\n——————————");

  // attinySerial.write(30);
  // delay(10);
  // uint8_t msg = attinySerial.read();
  // if (msg == 30)
  // {
  //   Serial.println("UART good");
  // }
  // else
  // {
  //   Serial.println(msg);
  //   Serial.println("UART not working");
  //   return;
  // }

  // attinySerial.write(5);
  // if (receive() != 5)
  // {
  //   Serial.println("Pin 5 damages UART");
  //   return;
  // }

  // attinySerial.write(8);
  // if (receive() != 8)
  // {
  //   Serial.println("Pin 8 damages UART");
  //   return;
  // }

  // pinTests
  for (int i = 0; i < 15; i++)
  {
    if (i != 4 && i != 5 && i != 6)
    {
      uint8_t picoPin = i;
      uint8_t attinyPin = pogo_pins[picoPin];

      attinySerial.write(attinyPin);
      delay(40);
      readPins(picoPin);
      delay(20);
      attinySerial.read();
      delay(100);
    }
  }
}

void setup()
{
  attinySerial.begin(9600);

  Serial.begin(9600);
  for (int i = 0; i < 15; i++)
    if (i != 5 && i != 4)
      pinMode(i, INPUT_PULLUP);
}

void loop()
{
  fullTest();
  delay(5000);
}
