# Embedded C Calculator on TM4C123GH6PM

This repository contains an embedded calculator project developed in C using Keil uVision5 and the TM4C123GH6PM microcontroller. The calculator uses a physical keypad for input and an LCD for output, allowing users to enter and evaluate arithmetic expressions.

The project was developed as part of my ELEC3662 embedded systems coursework and focuses on low-level embedded programming, modular firmware design, hardware interfacing, debugging and expression evaluation.

## Project Overview

The aim of the project was to design and implement a microcontroller-based calculator capable of performing basic arithmetic operations:

- addition
- subtraction
- multiplication
- division

A key requirement was that the calculator should follow the correct mathematical order of operations. To achieve this, I implemented an expression-processing workflow using Reverse Polish Notation and the shunting-yard algorithm.

The calculator accepts input in normal infix notation, converts the expression into postfix notation, and then evaluates the postfix expression using stack-based logic.

## Hardware and Platform

- TM4C123GH6PM microcontroller
- Keil uVision5 development environment
- SPLC780D LCD
- Matrix keypad
- Breadboard-based hardware implementation

## Main Features

- Keypad input scanning and decoding
- LCD initialisation and display control
- 4-bit LCD communication based on datasheet requirements
- SysTick-based delay functions
- Arithmetic expression input
- Infix-to-postfix conversion using the shunting-yard algorithm
- Reverse Polish Notation evaluation
- Stack and queue-style array manipulation
- BIDMAS/order-of-operations support
- Decimal number handling
- Negative number handling in supported cases
- ANS functionality to reuse the previous answer
- Shift-key functionality for accessing additional operators and commands
- Backspace and clear functionality

## Expression Evaluation Method

The calculator uses the following process:

1. The user enters an expression using the keypad.
2. The input is stored as a character array.
3. The infix expression is converted to postfix notation using the shunting-yard algorithm.
4. The postfix expression is evaluated using stack-based logic.
5. The result is converted into a displayable string.
6. The output is printed to the LCD.

This approach was chosen because it handles operator precedence more systematically than attempting to manually evaluate every possible arithmetic case.

## Code Structure

- `main.c`  
  Integrates the keypad, LCD, calculation logic and overall calculator workflow.

- `Calculations.c` / `Calculations.h`  
  Contains the main expression handling and calculation functions, including input collection, infix-to-postfix conversion, postfix evaluation and helper functions.

- `KeyPad.c` / `KeyPad.h`  
  Handles keypad initialisation, scanning, key reading and key decoding.

- `LCD.c` / `LCD.h`  
  Handles LCD initialisation, commands, character writing, string writing, cursor movement and display formatting.

- `PLL.c` / `PLL.h`  
  Configures the system clock for the TM4C123GH6PM microcontroller.

- `ELEC3362_Active_ch.uvprojx`  
  Keil uVision project file.

## Debugging and Development Process

Several issues were identified and resolved during development:

- The LCD data pins initially used the wrong 4-bit configuration, which was corrected after reviewing the LCD datasheet.
- The LCD contrast and LED backlight connections were debugged after incorrect voltage levels affected display behaviour.
- Keypad reading and decoding were separated into different functions to improve modularity and make the keypad module reusable.
- The expression evaluation logic was debugged to handle multi-digit numbers, decimals and selected negative-number cases.
- Custom conversion functions were implemented to avoid dependence on standard library functions.

## Limitations

The final implementation has some limitations:

- It does not fully handle all double-operator cases, such as `1--7`.
- Some negative-number cases are supported, but not every possible expression format.
- The planned sleep-mode feature was not implemented due to time constraints.

## Future Improvements

Possible future improvements include:

- expanding negative-number handling
- improving double-operator detection
- adding more mathematical functions
- implementing sleep mode using interrupts
- storing display or calculator state using EEPROM
- improving error handling for invalid expressions

## How to Run

1. Open the Keil project file in Keil uVision5.
2. Build the project.
3. Flash the program to the TM4C123GH6PM microcontroller.
4. Connect the keypad and LCD according to the project circuit.
5. Use the keypad to enter expressions and view results on the LCD.
