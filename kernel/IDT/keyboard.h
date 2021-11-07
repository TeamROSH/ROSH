#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "time.h"
#include "../ports.h"
#include "../../libc/screen.h"

#define KEYBOARD_INPUT_PORT 0X60
#define KEYBOARD_OUTPUT_PORT 0x20

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
#define CTRL_PRESS 0x11
#define SHIFT_PRESS 0xF
#define ALT_PRESS 0x12
#define CAPSLOCK_PRESS 0x13
#define CTRL_RELEASE 0x9D
#define SHIFT_RELEASE 0xAA
#define ALT_RELEASE 0xB8


/*
    This function handels keyboard input
    registers: registers values 
*/
void keyboard_handler(registers_t* registers);

/*
    This function parses keyboard input symbol into ascii
    input_symbol: keyboard symbol
    return: ascii representation 
*/
uint8_t symbol_to_ascii(uint8_t input_symbol);

/*
    This function initializes the keyboard symbol array
*/
void keyboard_initialize();

/*
    This function checks whether the char is printable and prints it
    input_char: the charater that will be checked and printed
*/
int keyboard_putc(uint8_t input_char);

#endif
