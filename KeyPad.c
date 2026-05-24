//KeyPad.c
#include "KeyPad.h"
// ***** 1. Pre-processor Directives Section *****
// PORT E
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_PDR_R				(*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control

// PORT D
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD     0x00000008  // port D Clock Gating Control
// ************************************************

// USE BIT-SPECIFIC ADDRESSING FOR MORE EFFICIENCY RATHER THAN MASKING THE ENTIRE REGISTER
// BIT-SPECFIC Port E 
#define inputE0      (*((volatile unsigned long *)0x40024004))
#define inputE1      (*((volatile unsigned long *)0x40024008))
#define inputE2      (*((volatile unsigned long *)0x40024010))
#define inputE3      (*((volatile unsigned long *)0x40024020))
// define all the columns in Port D individually
#define outputD0      (*((volatile unsigned long *)0x40007004))
#define outputD1      (*((volatile unsigned long *)0x40007008))
#define outputD2      (*((volatile unsigned long *)0x40007010))
#define outputD3      (*((volatile unsigned long *)0x40007020))
	
// ***** 2. Global Declarations Section *****
// header file
// ******************************************


// keypad matrix 
// COL1     COL2    COL3    COL4
//  [1]     [2]     [3]     [A]
//  [4]     [5]     [6]     [B]
//  [7]     [8]     [9]     [C]
//  [*]     [0]     [#]   	[D]

// keypad matrix when not shifted
// 			 COL1     COL2    COL3    COL4
//  ROW1 [1]     [2]     [3]     [+]
//  ROW2 [4]     [5]     [6]     [-]
//  ROW3 [7]     [8]     [9]     [.]
//  ROW4 [=]     [0]     [del]   [S]

//keypad when shhifted 
// 			 COL1     COL2    COL3    COL4
//  ROW1 [1]     [2]     [3]     [x]
//  ROW2 [4]     [5]     [6]     [/]
//  ROW3 [7]     [8]     [9]     [E]
//  ROW4 [=]     [0]     [clr]   [S']

// ***** 3. Subroutines Section *****
unsigned char shift = 0;  // 0: Unshifted, 1: Shifted // initialise to unshifted 

unsigned char readKeypad(void){ 
//	In = GPIO_PORTE_DATA_R&0x0F; // read PE3-0 into In // since only the first 4 ports are used,  bit-specific addressing will be more efficient
	//READ THE CORRESPONDING BYTE INTO COLUMN AND SET THE REST TO ZERO
	outputD0 = 0x01; outputD1 = 0x00; outputD2 = 0x00; outputD3 = 0x00;
	SysTick_Wait(20);
	if(inputE0 == 0x01){ return '1';} 
	else if(inputE1 == 0x02){/*lcdWriteData(0x34);*/return '4';}
	else if(inputE2 == 0x04){/*lcdWriteData(0x37)*/;return '7';}
	else if(inputE3 == 0x08){return '*';} // USED FOR End Input (EQUAL TO)
	//COLUMN 2
	outputD0 = 0x00; outputD1 = 0x02; outputD2 = 0x00; outputD3 = 0x00;
	SysTick_Wait(20);
	if(inputE0 == 0x01){/*lcdWriteData(0x32);*/return '2';}
	else if(inputE1 == 0x02){/*lcdWriteData(0x35);*/return 0x35;}
	else if(inputE2 == 0x04){/*lcdWriteData(0x38);*/return '8';}
	else if(inputE3 == 0x08){/*lcdWriteData(0x30);*/return '0';}
	//COLUMN 3
	outputD0 = 0x00; outputD1 = 0x00; outputD2 = 0x04; outputD3 = 0x00;
	SysTick_Wait(20);
	if(inputE0 == 0x01){/*lcdWriteData(0x33);*/return '3';}
	else if(inputE1 == 0x02){/*lcdWriteData(0x36);*/return '6';}
	else if(inputE2 == 0x04){/*lcdWriteData(0x39);*/return '9';}
	else if(inputE3 == 0x08){return '#';}	// DELETE ENTIRE ENTRY 
	//COLUMN 4
	outputD0 = 0x00; outputD1 = 0x00; outputD2 = 0x00; outputD3 = 0x08;
	SysTick_Wait(20);
	if(inputE0 == 0x01){/*lcdWriteData(0x2A);*/	return 'A';}
	else if(inputE1 == 0x02){/*lcdWriteData(0x3A);*/return 'B';}
	else if(inputE2 == 0x04){/*lcdWriteData(0x45);*/return 'C';}
	else if(inputE3 == 0x08){return 'D'; }	// USED AS SHIFT
	else{return 0;}			
}

unsigned char decode_key(unsigned char pressed_key){
		unsigned char decoded_val;
	// collects D input from readkeypad functuion and toggle the shift variable
	 if (readKeypad() == 'D') {  // Shift key
		 us_delay(40000); // MORE TIME FOR SHIFT TO BE READ 
        shift = 1;  // Toggle shift state
    } 
	if(shift==1){
		switch(pressed_key){
			case 0:
				decoded_val = 'X'; // this sets a default value for when no key is pressed
													// which will then be used as a condition to do nothing in the get input function
				break;
			case '1':
					decoded_val = 'A'; // WANT TO USE A FOR STORED ANSWER
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '2':
					decoded_val = '2';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '3':
					decoded_val = '3';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '4':
					decoded_val = '4';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case 0x35:
					decoded_val = 0x35;
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '6':
					decoded_val = '6';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '7':
					decoded_val = '7';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '8':
					decoded_val = '8';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '9':
					decoded_val = '9';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '0':
					decoded_val = '0';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '*':
					decoded_val = '=';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '#':
					decoded_val = 'D';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case 'A':
					decoded_val = 'x';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case 'B':
					decoded_val = '/';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case 'C':
					decoded_val = 'E';
					shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
		}
		
	} else if(shift==0){
		
		switch(pressed_key){
			case 0:
				decoded_val = 'X'; break;
			case '1':
					decoded_val = '1';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '2':
					decoded_val = '2';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '3':
					decoded_val = '3';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '4':
					decoded_val = '4';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case 0x35:
					decoded_val = 0x35;
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '6':
					decoded_val = '6';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '7':
					decoded_val = '7';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '8':
					decoded_val = '8';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '9':
					decoded_val = '9';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '0':
					decoded_val = '0';
					//shift = 0;	// have to toggle shift back to zero after each decoding branch
					break;
			case '*':
					decoded_val = '=';
					break;
			case '#':
					decoded_val = 'R';
					break;
			case 'A':
					decoded_val = '+';
					break;
			case 'B':
					decoded_val = '-';
					break;
			case 'C':
					decoded_val = '.'; //decoded_val = 0x2E;
					break;}	
	}
return decoded_val;}













// ROWS FOR INPUT 
void PortE_Init(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;     // 1) E clock
  delay = SYSCTL_RCGC2_R;           // delay 
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE PF0  
  GPIO_PORTE_CR_R |= 0x0F;           // allow changes to PE1-0       
  GPIO_PORTE_AMSEL_R &= 0x00;//~0x0F;         // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R &= ~0x0F;     // 5) PE3-0 as inputs (0)  // the other ports should not be affected
  GPIO_PORTE_AFSEL_R &= 0x00;        // 6) no alternate function
  //GPIO_PORTE_PUR_R |= 0x00;          // NO PUR ///
	// IMPLEMENT PDR LATER
	GPIO_PORTE_PDR_R |= 0x0F;          // PULL DOWN RESISTORS FOR ALL THE INPUTS ///
  GPIO_PORTE_DEN_R |= 0x0F;          // 7) enable digital pins PF4-PF0  ///   enable digital pins PE1-PE0  
  //GPIO_PORTE_DATA_R |= 0x0F;  
}
// COLUMNS FOR OUTPUTS
void PortD_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;     // 1) D clock
  delay = SYSCTL_RCGC2_R;           // delay 
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock PortD  
  GPIO_PORTD_CR_R |= 0x0F;           // allow changes to PD1-0       
  GPIO_PORTD_AMSEL_R &= 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R |= 0x0F; //         // 5) PD3-0 AS INPUTS
  GPIO_PORTD_AFSEL_R &= 0x00;        // 6) no alternate function
  //GPIO_PORTE_PUR_R |= 0x00;          //  No PUR
  GPIO_PORTD_DEN_R |= 0x0F;          // 7) enable digital pins PF4-PF0  ///   enable digital pins PE1-PE0  
  //GPIO_PORTE_DATA_R |= 0x0F;  
}

void keypadInit(){
	PortD_Init();
	PortE_Init();
}
// **********************************









