#include "keyboard.h"

uint8_t g_symbol_arr = {
  UNKNOWN_SYMBOL,   ESCAPE_SYMBOL, '1',  '2',  '3',  '4',  '5',  '6', 
  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  
  'o',  'p',  '[',  ']',  '\n', UNKNOWN_SYMBOL,   'a',  's',
  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  
  '\'', '`',  UNKNOWN_SYMBOL,   '\\', 'z',  'x',  'c',  'v',
  'b',  'n',  'm',  ',',  '.',  '/',  UNKNOWN_SYMBOL,   '*',  
  UNKNOWN_SYMBOL,   ' ',  UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,
  UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   '7',  
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,   UNKNOWN_SYMBOL,  
  [0x49] = PAGEUP_SYMBOL, [0x51] = PAGEDOWN_SYMBOL, [0x47] = HOME_SYMBOL, [0x4F] = END_SYMBOL, [0x52] = INSERT_SYMBOL, [0x53] = DELETE_SYMBOL, [0x48] = UP_SYMBOL, [0x50] = DOWN_SYMBOL,
  [0x4B] = LEFT_SYMBOL, [0x4D] = RIGHT_SYMBOL, [0x9C] = '\n', [0xB5] = '/'
};

uint8_t g_keyboard_input[NUM_OF_SYMBOLS];

void keyboard_handler();
uint8_t patse_to_ascii(uint8_t input_symbol);


uint8_t patse_to_ascii(uint8_t input_symbol)
{
    return g_symbol_arr(input_symbol);
}

void keyboard_handler()
{
    uint8_t input_symbol = inb(KEYBOARD_INPUT_PORT);
    if(input_symbol < 0x80 || input_symbol == 0x9C || input_symbol == 0xB5)
    {

    }
}
