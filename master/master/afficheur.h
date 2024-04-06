#include <TM1637Display.h>

// TM1637Display : setSegments "dEFU"
// constant defu :

const uint8_t defu[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G, // d
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, // E
  SEG_A | SEG_E | SEG_F | SEG_G, // F
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F// U
};

const uint8_t lost[] = {
  // fed
  SEG_F | SEG_E | SEG_D, // L
  // gedc
  SEG_G | SEG_E | SEG_D | SEG_C, // O
  // afgcd
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D, // S
  // fedg
  SEG_F | SEG_E | SEG_D | SEG_G // T
};