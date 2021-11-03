#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../ports.h"

#define KEYBOARD_INPUT_PORT 0X60
<<<<<<< HEAD
#define KEYBOARD_OUTPUT_PORT 0x20
=======
>>>>>>> 570d96e6c927c9943c79eff113b49735d364c608

#define UNKNOWN_SYMBOL 0
#define NUM_OF_SYMBOLS 256

#define ESCAPE_SYMBOL       0x1B
#define HOME_SYMBOL        0xE0
#define END_SYMBOL         0xE1
#define UP_SYMBOL          0xE2
#define DOWN_SYMBOL        0xE3
#define LEFT_SYMBOL        0xE4
#define RIGHT_SYMBOL       0xE5
#define PAGEUP_SYMBOL      0xE6
#define PAGEDOWN_SYMBOL    0xE7
#define INSERT_SYMBOL      0xE8
#define DELETE_SYMBOL      0xE9

/*
    This function handels keyboard input 
*/
void keyboard_handler();

/*
    This function parses keyboard input symbol into ascii
    input_symbol: keyboard symbol
    return: ascii representation 
*/
uint8_t patse_to_ascii(uint8_t input_symbol);

<<<<<<< HEAD
/*
    This function initializes the keyboard symbol array
*/
void keyboard_initialize();

/*
    This function checks whether the char is printable and prints it
    input_char: the charater that will be checked and printed
*/
void keyboard_putc(uint8_t input_char);

#endif
=======
#endif
>>>>>>> 570d96e6c927c9943c79eff113b49735d364c608
