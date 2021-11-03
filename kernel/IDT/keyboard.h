#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../ports.h"

#define KEYBOARD_INPUT_PORT 0X60

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
    
*/
void keyboard_handler();

#endif