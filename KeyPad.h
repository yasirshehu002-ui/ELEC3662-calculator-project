
#ifndef KEYPAD_H
#define KEYPAD_H
#include "PLL.h"
#include "LCD.h"

// Keypad Function Prototypes
void PortE_Init(void);
void PortD_Init(void);
void keypadInit(void);
unsigned char readKeypad(void);
unsigned char decode_key(unsigned char pressed_key);
#endif


