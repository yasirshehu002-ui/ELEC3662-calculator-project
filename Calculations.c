// calculations.c
#include "KeyPad.h"
#include "LCD.h"
#include "Calculations.h"
// try implementing my_atof with switch-case for readability and my personal preference over multiple if-else baranches
float my_atof(const char *element) {
    float result = 0.0; 
    int whole_no=0, neg = 0;    // whole number of the number// Flag for negative numbers 
    float dec = 0.0; // decimal part of the number
    int place_value = 1;    // store place vlaue that will be used to divide 
    while (*element) {// Iterate through the characters of the string
        switch (*element) {
            case '-':
                // Handle negative numbers
                neg = 1;
                element++; // Move to the next character
                break;
            case '.':
                // Check for a decimal point
                element++; // Skip the decimal point // because I want to manipulate the fractional part after the point
                while (digit(*element)) { // at this point i can then store the fractional part in the variable
                    dec = dec * 10 + (*element - 48); // same ASCII manipulation as the integer part
                    place_value *= 10; // Increment the divisor// this increments per digit contained in the fractional part
                                   // so .223 will correspond to 100, which will be used do divde later
                    element++; // next character
                }
                break;
            default:
                // Process the integer part
                if (digit(*element)) {
                    whole_no = whole_no * 10 + (*element - 48); // ASCII representation of 0 is 48
                                                            // for example 2 (i.e 50) will be (50-48) == (2-0)
                    element++; // move to next element
                } else {
                    // Stop processing on an invalid character
                    return 0.0;
                }
                break;
        }
    }
    result = whole_no + (dec / place_value);// arithmetically Combine whole and fractional parts 
    if (neg) {
        result = -1 * result;
    }return result;}



const char *my_sprintf(float input_float, int precision) { //inspiration from int_to_string in https://www.youtube.com/watch?v=1Wy2fG1T0aU
    static char buffer[32]; // Static buffer to hold the output string
    char *p = buffer + sizeof(buffer) - 1; // pointer to the location of element in the string
																							// reverse indexing so i can decrement the pointer from left to right in the string 
    *p = '\0'; // Null-terminate the string, loop will begin decrementing from the terminator
    int negflag = 0;
		float dec= 0.0;
		int whole_no,casted_dec = 0; // whole number; decimal part casted to integer 
	
    if (input_float < 0) {negflag = 1;// convert to positve, negative is appende after conversion
        input_float = (-1) * input_float; 
    }
    // Separate whole and decimal parts
     whole_no = (int)input_float;
     dec = input_float - whole_no;
		
    // Process the fractional part
    for (int i = 0; i < precision; i++) {dec = dec*10;} 
        // for every iteration a multiple of 10 is added, increasing the decimal placed
     casted_dec = (int)(dec + 0.5f); // approx to the nearest hwole number then cast to integer

		
		// do while was used to handle boundary condition. 
		// because when i tried to convert "0" it did not work due to the condition on the while loop not being fulfiled
		// so and initial iteration of the code block was needed first before then checking the condition for the logic 
    // Convert fractional part to string
    do {
        *--p = (casted_dec % 10) + 48;// obtains the last digit of the decimal part then adds to 0// ASCII 0 is 48; 
        casted_dec /= 10;
    } while (casted_dec > 0);

    *--p = '.'; // Add decimal point

    // Convert integer part to string, till first element of srings is reached
    do {
        *--p = (whole_no % 10) + 48; 
        whole_no /= 10;
    } while (whole_no > 0);

    // Add negative sign if necessary
    if (negflag == 1) {
        *--p = '-'; // append "-" to the begining of the strng
    }return p; // Return the pointer to the beginning of the string
}

 float eval_stack[32]; // stack  for evaluation function
 int eval_top = -1;				//  stack pointer for evaluation function

int digit(char c) { // function to return whaever digit is passed in as the attribute
    return (c >= '0' && c <= '9');
}

unsigned long exp_base10(float exponent) { // use for E casen later
    unsigned long result = 1;
    // Iteratively compute 10^exponent
    for (int i = 0; i < exponent; i++) {
        result = result * 10; // mulitplies 1 by 10 stores it back in the result variable per iteration
											// the number if iterations is determined by the input 
    }
    return result;
}

int operator_order(char op) {
    switch (op) {
        case '+': return 1;  // Lowest 
        case '-': return 1;  
        case 'x': return 2;  
        case '/': return 2;  
        case 'E': return 3;  // Highest 
        default:  return 0;  // when no op is detected
    }
}



// Write a floating-point number (decimal) to the LCD(already converted to string in function)
// works similar to lcdWriteString() function
void lcdWriteFloat(float number, int precision) {
    const char *string = my_sprintf(number, precision);
    // Iterate through the resulting string and display each character
    while (*string) { // Loop until the code sees "/0"
        lcdWriteData(*string); // Send each character to the LCD by abstracting SendDisplayByte()
        us_delay(40);  //
        string++;    // Move to the next character
    }
}


// Push a value onto the stack
void push(float val) {
    if (eval_top >= 63) { // if more than implemented array size 
        lcdWriteString("Er->StackOvf"); //  error handling for overflow
        return;
    }
   else{ eval_stack[++eval_top] = val;} // ++eval_top ensures the variable is moved to the next postion before storing
}


// Pop from the stack
float pop() {
    if (eval_top < 0) {
        lcdWriteString("Er->emptystack"); // error handling for empty stack for debugging
        return 0;
    }
   else if (eval_top>0){ return eval_stack[eval_top--];} // returns the "top" value of the stack then decrements it
																												// indicating that the topmost element has been popped
}


// Check if the stack is empty
int isEmpty(int top) {
    switch (top) {
        case -1:
            return 1; // Stack is empty, -1 used as an invalid index for the array 
        default:
            return 0; // Stack is not empty
    }
}




void get_input(char* expression, int* cursor_position, float* memory, int* answer_flag) { //the attributes are stores as pointers so thta the memory 
																												// locations can be manipulated directly within the scope of the fucntion
    unsigned char input; // variable to store whatever the LCD decoded
		int use_memory_flag = 0;

    while (1) {  // LOOP UNTIL  '=' is pressed
        input = decode_key(readKeypad()); // Read and decode input
        ms_delay(80); //debounce delay

        switch (input) {
					
            case '=':  // End of expression
                expression[*cursor_position] = '\0'; // Null-terminate the array
                                                    // required for indexing when manipulating the array
                                                    // because of how the array datatype in C works
                return; // Exit the function

							

            case 'X': 
                // if nothing is pressed, do nothing and continue
                // defaults the keypad code to break when nothing is pressed
                // this was to avoid the code to keep running and printing values to the LCD 
                // after a button is released and nothing is pressed
                break;

            case 'R':  // Backspace functionality
                if (*cursor_position > 0) {
                    (*cursor_position)--;              // Move cursor back
                    expression[*cursor_position] = '\0'; // Remove (rubout) last character by replacing with null terminator
                    lcdSendCommand(shiftCursor_L);     // Move cursor left
                    us_delay(40);                      // Delay
                    lcdWriteData(' ');                 // Clear the character on the LCD
                    us_delay(40);                      // Delay
                    lcdSendCommand(shiftCursor_L);     // Move cursor back again
                    us_delay(40);                      // Delay
                }
                break;

            case 'D':  // Clear display functionality
                // simply clears the content of the screen
                // a more efficient way to clear the display while still having the content stored 
                // using EEPROM would be explored if time is sufficient
                lcdSendCommand(clearDisplay); // Clear LCD
                lcdSendCommand(returnHome);   // Reset cursor to home
                *cursor_position = 0;         // Reset cursor position
                for (int i = 0; i < 64; i++) expression[i] = '\0'; // Clear the array
                                                                    // the array size was defaulted to 64; more than enough for the desired implementation            
                break;
						
						case 'A':  // User pressed 'A' to recall previous answer
                { 
                    const char *mem2string = my_sprintf(*memory, 4);  // Convert memory float to string using my_sprintf
						
                    int j = 0;
                    while (mem2string[j] != '\0') {  
                        expression[*cursor_position] = mem2string[j];  
												lcdWriteData(mem2string[j]);  // Display stored answer
                        (*cursor_position)++;  
                        j++;}
                }
                use_memory_flag =1;
								break;

            default:  // Handles valid characters: digits and operators
                if (digit(input) || input == '+' || input == 'x' || input == '-' ||
                    input == '/' || input == '.' || input == 'E') { 	
                    expression[*cursor_position] = input; // Add key to expression
                    (*cursor_position)++; // Update cursor position VARIABLE. actual cursor on LCD auto shifts 
                                           // see initialization process for entry mode set
                    lcdWriteData(input); // Display on LCD
										use_memory_flag = 1; 
                 
                }
                break;
        }
					
       // while ((*((volatile unsigned long *)0x400243FC)) & 0x0F); // WHILE PORT E IS "SOMETHING" DO NOTHING THEN CONTINUE
                                                                 // prevents bouncing or unintended presses
																																// look for more efficient way later
        SysTick_Wait(5000000); // display was updating too quickly, so I added this 
    }
}


 //base code

void infix_to_Postfix(const char *infix, char *postfix) {
    char op_stack[64];     // Stack array to store the operators 
    int k = -1;            // for tracking the stack
    int j = 0;             // Index for tracking the postfix

    for (int i = 0; infix[i] != '\0'; i++) {
        char val = infix[i]; // Current iteration's character from infix

        // for a case like -3+5; the logic interprets it as 0-3+5
        if (val == '-' && (i == 0 || (infix[i - 1] == '+' || infix[i - 1] == '-' || infix[i - 1] == 'x' // i==0 to chack first input
					|| infix[i - 1] == '/' || infix[i - 1] == 'E'))) {
            postfix[j++] = '0';  
        }	// implement for x--y if time

        switch (val) {
            // changed to switch-case for code uniformailty
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '.': 
                postfix[j++] = val; // Add operand directly to postfix
                break;

            // BIDMAS for operators 
            case '+': case '-': 
            case 'x': case '/': 
            case 'E': // if val is any of these cases, then
                postfix[j++] = ' '; // Add a separation between operands
                while (!isEmpty(k) && operator_order(op_stack[k]) >= operator_order(val)) { 
                    postfix[j++] = op_stack[k--]; // Pop higher/equal precedence operator
                    postfix[j++] = ' '; // Add space for separation
                }
                op_stack[++k] = val; // Push current operator onto stack
                break;

            default:
                // Handle unexpected characters
                lcdWriteString("Er->InvalidChar"); // error handling
                return;
        }
    }

    // Pop any remaining operators from the stack
    while (!isEmpty(k)) {
        postfix[j++] = ' ';       // Add space to separate the operands before popping
        postfix[j++] = op_stack[k--]; // then pop
    }

    postfix[j] = '\0'; // Null-terminate the postfix expression
}




float calc(const char *postfix, int *answer_flag) { // expression must be a postfix
    char buffer_2[64]; // Buffer for extracting numbers
    int value_position; // to track the pos of numbers (not digits) in the buffer

    for (int i = 0; postfix[i] != '\0'; i++) { // anything but the null-terminator's ASCII representation
        // Skip spaces.
        if (postfix[i] == ' ') continue;  // space was used to seperate the posftix values in the array
																					// but need to be ignored for evaluation

        // numbers; with decimal consideration
        if (digit(postfix[i]) || postfix[i] == '.') {
            value_position = 0; // begin storing from index 0
            while (postfix[i] != ' ' && postfix[i] != '\0') {
                if (!digit(postfix[i]) && postfix[i] != '.') {
                    lcdWriteString("E->Invalidinput"); // error handling. unlikely to happen because my keypad will always give a valid digit based on my checker
                    return 0;													// but useful in debugging!
                }
                buffer_2[value_position++] = postfix[i++]; // increment the number tracker to handle the next position
																										// i do not want the present position because i still null-reminate the array after pushing a number onto it 
            }
            buffer_2[value_position] = '\0'; // null terminate the buffer 
            push(my_atof(buffer_2)); // Convert number to float and push onto stack //use atof first before fixing bugs in my_atof()
            i=i-1; // dec i for next iteration so it does not skip a following operator, because before this point the counter was pointing to the null-term or space
            continue; // continue to next character
        }

        // operators
        if (eval_top < 1) {
            lcdWriteString("E->MathErr");// error handling for absent number inputs
																							//  re-implement this to allow for -ve number inputs, like -1--1 = 0 if time
            return 0;
        }

        float y = pop(); // must be the second number first because i want to store it in the operand THEN operate on it 
        float x = pop();
				
				if (postfix[i] == '-' && postfix[i-1] == '0' && postfix[i-2] == ' ') {  // check if i have a case like " 0-"
    push((0-y)); // then push 
} 

        switch (postfix[i]) {
						case 'x': push(y*x); break; //case 'x': push(multiply(a,b)); break;// multiply fucntion is buggy, debug snd use if time allows
            case '+': push(x + y); break;
            case '-': push(x - y); break;
            case '/': 
                if (y == 0) { // cannot divide by zero
                    lcdWriteString("E->ZeroErr"); // error handling for zero error 
                    return 0;
                }
					else if (x==0){push(0.0 + 0.0);} // push zero ddirectly because compiler is not returning 0/5 as 0;// debug for more efficient error handling if possible
                else{push(x / y);}
                break;
								
            case 'E': if(y > 31){lcdWriteString("Er->expTooHigh"); // error handling for maximum float exponent value
                    return 0;}
						
                push(x * exp_base10(y)); // buggy when trying to handle float exponent, fix if time allows
                break;
            default:
                lcdWriteString("Er->InvalidOpp"); // error handling
                return 0;
        }
    }
		*answer_flag = 1;
    return pop(); // pop the last (and only) value remaining in the stack
}
