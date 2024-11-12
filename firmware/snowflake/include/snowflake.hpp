#include <Arduino.h>

class Snowflake {

public:
  static void init();
  static void randomAnimation();
  static void circularAnimation(uint8_t divider);
  static void light(bool on);
  static void clear();

private:
  static void _shiftSingle(bool data);
  static void _shiftAll(uint8_t *data);
};