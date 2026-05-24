//LCD.c

// ***** 1. Pre-processor Directives Section *****
// define the commands from the datasheets to make them easier to use accross different .c files
//1. clear display
#include "LCD.h"
#define RS (*((volatile unsigned long *)0x40004020)) //RS - PA3
#define EN (*((volatile unsigned long *)0x40004010)) //EN - PA2
#define DB (*((volatile unsigned long *)0x4000503C)) //DB7-4 are mapped to PB3-0 (bit-specific for efficiency)
//PortA
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
//PortB
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R      (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	

void lcdInit(void){
	//us_delay(40000);
	ms_delay(40); // wait for 50ms
	//EN = 0x00;
	SendDisplayNibble(0x00,0x03); //0011 command
	us_delay(4100); // wait for 4.1ms//////
	SendDisplayNibble(0x00,0x03); //0011 command
	us_delay(100); // wait for 100us
	SendDisplayNibble(0x00,0x03); //0011 command

	us_delay(40);//us_delay(37); // not given in datasheet but 37us will suffice 
	SendDisplayNibble(0x00,0x02); //0010 command; toggling the LSB now sets it in 4-bit mode
															  //(collecting input from DB7-4)
	
  //LcdSendCommand can now be used to instruct the LCD going forward 
	lcdSendCommand(0x02);
  lcdSendCommand(0x01); // clear display
	lcdSendCommand(0x06); // entry mode set
	
	SysTick_Wait(500000);
	// delays were added as suggested by Dr Jones
	lcdSendCommand(0x28); //// 0x2B also works; DL=0 - 4-bit mode, N=1 - 2 lines, F=0 - 5x8
   ms_delay(2);	
	lcdSendCommand(0x06); // entry mode set
	   ms_delay(2);
		lcdSendCommand(0x0F);	// display on, cursor on, blinks cursor*/
		   ms_delay(2);
	lcdSendCommand(0x03);	// Return Home
}


void PortB_Init(void){ volatile unsigned long delay;
	
  SYSCTL_RCGC2_R |= 0x00000002;      // 1) PORTB clock
  delay = SYSCTL_RCGC2_R;            // delay   
	GPIO_PORTB_CR_R |= 0x0F;           // 2) allow changes to PB3-0  
  GPIO_PORTB_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTB_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R |= 0x0F;          // 4) PB3-0 are  outputs
  GPIO_PORTB_AFSEL_R &= 0x00;        // 5) no alternate function   
  GPIO_PORTB_DEN_R |= 0x0F;          // 6) enable digital pins PB3-0
}
void PortA_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000001;      // 1) PORTA clock
  delay = SYSCTL_RCGC2_R;            // delay       
	GPIO_PORTA_CR_R |= 0x0C;           // 2) allow changes to PA3-2 
  GPIO_PORTA_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTA_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R |= 0x0C;          // 4) PA3-2 are the outputs
  GPIO_PORTA_AFSEL_R &= 0x00;        // 5) no alternate function   
  GPIO_PORTA_DEN_R |= 0x0C;          // 6) enable digital pins PA3-2
	}
 void initPortslCD(void){
	 PortA_Init();
	 PortB_Init();
}

void lcdENPulse(void){ // to toggle enable pin //  need to pulse for 450ns
	EN = 0x04;
	//SysTick_Wait(2);//from datasheet (P21) we know E needs 25ns maximum to rise before we can even pulse it
	SysTick_Wait(20); // 450ns pulse required
	EN = 0x00; 
	SysTick_Wait(20); // 
}
void SendDisplayNibble(unsigned char dataflag, unsigned char nibble) {
	// first read the value for dataflag into RS register; 0x00 for command, 0x08(i.e setting PA3 high) for data
	RS = dataflag;  
	SysTick_Wait(4);
	DB = nibble;
  lcdENPulse();
}
void SendDisplayByte(unsigned char dataflag, unsigned char byte){ // an extension of send display nibble 
	
	SendDisplayNibble(dataflag,((byte&0xF0) >> 4));// send upper nibble 4-7; make 0-3 zeros, leave 4-7 the same, then shift the zeros away
	us_delay(1);
	SendDisplayNibble(dataflag,(byte&0x0F));	// send lower nibble 0-3
	us_delay(400); // 37us delay for the display to act on what it has recieved 
}
// the below functions are just an extension of the sendDisplayByte function.
// an extra function call yes, but a higher level of abstraction, making the code easily debuggable and usable
void lcdWriteData(unsigned char byte){
	SendDisplayByte(0x08,byte);
}
void lcdSendCommand(unsigned char byte){
	SendDisplayByte(0x00,byte);
}

void lcdWriteString(char *string) {
	while(*string){ // keep looping while the input is not null ('\0'). compiler detects this with ASCII
		lcdWriteData(*string); // write string unto dsiplay
		us_delay(100); // there is already a delay in the previous function
									// this extra delay was added to facilitate for any functional overhead
									// so that the LCD has enough time to inteprete the code
		string++; // increment the ASCII value and move onto the next string
	}
}

void lcdMoveCursor(int shifts){ // shifts the cursor on the screen based on input
  while (shifts > 0) {
        lcdSendCommand(0x14); // Shift cursor right
        shifts--; // Decrement counter
    }
}
