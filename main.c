// main.c

// 1. Pre-processor Directives Section
#include "PLL.h"
#include "LCD.h"
#include "KeyPad.h"
#include "calculations.h"
#define row_1          0x80
#define row_2          0xC0

void PrintToDisplay(void);
void test_lcdWriteFloat(void);

int main(void){
	// initialisation section //
		PLL_Init();SysTick_Init();keypadInit();initPortslCD();lcdInit();
	  // Buffers for expressions
    char expression[64];  // Stores the infix expression; 64 because of the way char operates on the array
    char postfix[64];     // Stores the postfix expression
		// variables 
    float result;         // Stores the evaluated result 
    float test_result;         // Stores the evaluated result 
		int cursor_position = 0; // Tracks cursor position
	
		float stored_memory = 0.0; float *memory = &stored_memory;
		int answer_flag_val_in_mem = 0;int *answer_flag = &answer_flag_val_in_mem;
		SysTick_Wait(5000000);
		test_result = calc("1 4 /", 0);
	
    while (1) { // loop continuously
      get_input(expression, &cursor_position, memory, answer_flag); // collect input and store in array
			infix_to_Postfix(expression, postfix); // Convert infix to postfix
			
			lcdSendCommand(row_2);	us_delay(40); // Move cursor to the second line
			lcdMoveCursor(20);lcdWriteData('>'); lcdWriteData(' ');
		
			result =  calc(postfix, answer_flag);
			*memory = result;ms_delay(200);  // Store calculation result into memory
		if (result == 0) {}//lcdWriteString("ERROR"); // not needed again as error specific scenarios have been handled
		else if(result != 0) {
			for (int i = 0; i < 14; i++){ lcdWriteData(' ');} // Clear the line becuase sometimes the errors overwrite answers
						lcdSendCommand(0xC0);lcdMoveCursor(20);lcdWriteData('>'); lcdWriteData(' '); // formatting for the display
			us_delay(4000);
        lcdWriteFloat(result,4); // Display the result on the LCD
			}
		lcdSendCommand(row_1 + cursor_position); // Move to first row + position after previous expression
		}
}
	


	void test_lcdWriteFloat(void) {
    float testValues = 1234.676;
       // lcdSendCommand(clearDisplay);               // Clear the LCD before each test
        //lcdSendCommand(0x80);     // Move cursor to the first line
        lcdWriteString("Testing: "); // Display a label
        //lcdWriteFloat(testValues); // Write the test float value
        SysTick_Wait(5000000);    // Wait for some time to view the result
    }

	

	
	
	
	
	
	
	
	
	
	