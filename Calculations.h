#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// Function prototypes
void get_input(char* expression, int* cursor_position, float* memory, int* answer_flag); // Collects input expression from the keypad
// may not use memory elsewhere but still decalre as pointer if i have time to implement more features 
void infix_to_Postfix(const char* infix, char* postfix); // Converts infix to postfix notation
float calc(const char* postfix, int* answer_flag); // Evaluates the postfix expression
int operator_order(char op); // Determines the precedence of operators
//void lcdWriteFloat(float number); // Writes a floating-point number to the LCD
void lcdWriteFloat(float number, int precision); // Writes a floating-point number to the LCD
int digit(char c);
unsigned long exp_base10(float exponent);
void push(float value);
float pop(void);
float multiply(float a, float b);
int isEmpty(int top);
void floatToStr(float number, char* buffer);
float my_atof(const char *element); // i will iterate through the elements of the string
const char *my_sprintf(float number, int precision);

// extern used to delare the variable so that it can be used elsewhere
extern float eval_stack[32];
extern int eval_top;

// Operator stack for infix_to_Postfix
extern char op_stack[64];
extern int op_top; // Stack pointer for operators



#endif	  

