#include <Arduino.h>

class Snowflake {

public:
  static void init();
  static void randomAnimation();
  static void circularAnimation(uint8_t divider);
  static void light(bool on);
  static void clear();
  static void bitShiftPowerOff();
  static void bitShiftPowerOn();
  static void sleep();

private:
  static void _shiftSingle(bool data);
  static void _shiftAll(uint8_t *data);
};