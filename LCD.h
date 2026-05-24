#ifndef LCD_H
#define LCD_H

#define clearDisplay          0x01
#define returnHome          	0x03


#define shiftCursor_R					0x17
#define shiftCursor_L     		0x10
#define shiftDisplay_R   			0x1F
#define shiftDisplay_L    		0x18

//define entry mode set to increment cursor without shifting display as it would be a common command for printing on LCD
#define R_shift_no_display_shift	0x06
#include "PLL.h"

// initialisations 
void initPortslCD(void);
void PortB_Init(void);
void PortA_Init(void);
void lcdInit(void);

// basic functions 
void lcdENPulse(void);
void SendDisplayNibble(unsigned char dataflag, unsigned char nibble);
void SendDisplayByte(unsigned char dataflag, unsigned char byte); // will be an extension of the prev function
void lcdSendCommand(unsigned char byte);

// additional functions 
void lcdClearScreen(void);
void lcdMoveCursor(int shifts);
void lcdWriteData(unsigned char c);//
void lcdWriteString(char *string);
void clearDisplayScreen(void);
void moveDisplayCursor(char direction);

#endif
