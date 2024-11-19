#include <Arduino.h>

class Snowflake {
  // clang-format off
  enum LED {
    A = 1UL << 1, B = 1UL << 15, C = 1UL << 9, D = 1UL << 23,
    E = 1UL << 17, F = 1UL << 7, G = 1UL << 3, H = 1UL << 2,
    I = 1UL << 14, J = 1UL << 13, K = 1UL << 12, L = 1UL << 8,
    M = 1UL << 11, N = 1UL << 10, O = 1UL << 22, P = 1UL << 21,
    Q = 1UL << 20, R = 1UL << 16, S = 1UL << 19, T = 1UL << 18,
    U = 1UL << 6, V = 1UL << 5, W = 1UL << 4, X = 1UL << 0
  };
  // clang-format on

public:
  static void init();
  static void randomAnimation();
  static void circularAnimation(uint8_t divider);
  static void blinkAnimation();
  static void showBatteryState();
  static void spinningAnimation();
  static void outwardsAnimation();

  static void testAnimation();

  static void light(bool on);
  static void clear();
  static void bitShiftPowerOff();
  static void bitShiftPowerOn();
  static void sleep();

private:
  static void _shiftSingle(bool data);
  static void _shiftAll(uint8_t *data);
  static void _shiftAll_u24_t(uint32_t data);
  static void _fadeDelay(uint16_t ms);

  ////////////////////////////////////////////////////////////
  // LED mappings for easier symmetrical animation progamming:
  ////////////////////////////////////////////////////////////

  // LED index goes clockwise starting from the top LED
  static const LED outerCircle[18];
  static const LED innerCircle[6];

  // Branch index goes clockwise starting from the top branch
  // Branch LED index goes clockwise starting from the center LED
  static const LED branch[6][4];
};