# pld-comp

## Installation
- Install dependencies
>`sudo apt install openjdk-8-jre-headless`

>`sudo apt install clang`
- Build the project:
  - go to the src folder: 
  >`cd src`
  - build the project: 
  >`make`
  - run all tests: 
  >`make test`
    - to run all tests even when one fails: 
  >`make test -k`
- To clean the project before commit: 
  >`make clean`

## Usage
- In source folder, run the executable called "exe"
- Usage: `./exe inputfile.c [-a] [-s|-msp [outputFile]]`
  - `-a`: activate static analysis of the input file
  - `-o`: activate optimization (not yet implemented)
  - `-s [outputFile]`: generate x86 assembly code. `outputFile` is optional and defaults to `output.s`
  - `-msp [outputFile]`: generate assembly for MSP430 microcontroller instead of x86. This feature is not fully implemented and is only capable of generating very simple msp-430 code, with only declaration, definition, addition and subtraction of 32-bit integers.
 
## Features
 - types : int(32 bits) & char(8 bits)
 - variables of same type are stored in contiguous memory : no wasted space
 - arithmetic operations (+, -, \*, /) with priorities and parentheses on mixed int and/or chars
 - variable declaration anywhere 
 - variable simultaneous declaration & initialization 
 - assembly generation for x86 (advanced) and MSP430 (basic) - use of IR 
 - function definition and call with return value and up to 6 arguments (putchar, ...)
 - recursive functions
 - multiple nested if, else, while 
 - compare operations
 - ignore preprocessor directives
 - ignore comments
 - semantic error handling (double declaration, use of a variable before its declaration, wrong use of function)
 - syntactic error handling (via Antlr)
 
## Not implemented features 
 - other types than int/char
 - pointers and arrays
 - procedures (void functions)
 - logical operations, negation
 - for, do...while, switch...case
 - functions with more than 6 arguments
 - global variables
 - assignment returning a value
 - multiple files and imports
 - variable scope/range
 
