#include "keyboard.h"

uint8_t g_symbol_arr[NUM_OF_SYMBOLS];
uint8_t g_keyboard_input[NUM_OF_SYMBOLS];

void keyboard_handler();
uint8_t patse_to_ascii(uint8_t input_symbol);
void keyboard_initialize();

void keyboard_initialize()
{
    for(int i = 0; i < NUM_OF_SYMBOLS; i++)
    {
        arr[i] = UNKNOWN_SYMBOL;
    }

    g_symbol_arr[0x2] = '1'; g_symbol_arr[0x3] = '2'; g_symbol_arr[0x4] = '3';
    g_symbol_arr[0x5] = '4'; g_symbol_arr[0x6] = '5'; g_symbol_arr[0x7] = '6';
    g_symbol_arr[0x8] = '7'; g_symbol_arr[0x9] = '8'; g_symbol_arr[0xA] = '9';
    g_symbol_arr[0xB] = '0'; g_symbol_arr[0xC] = '-'; g_symbol_arr[0xD] = '=';
    g_symbol_arr[0xE] = '\b'; g_symbol_arr[0x10] = 'q'; g_symbol_arr[0x11] = 'w';
    g_symbol_arr[0x12] = 'e'; g_symbol_arr[0x13] = 'r'; g_symbol_arr[0x14] = 't';
    g_symbol_arr[0x15] = 'y'; g_symbol_arr[0x16] = 'u'; g_symbol_arr[0x17] = 'i';
    g_symbol_arr[0x18] = 'o'; g_symbol_arr[0x19] = 'p'; g_symbol_arr[0x1A] = '[';
    g_symbol_arr[0x1B] = ']'; g_symbol_arr[0x1C] = '\n'; g_symbol_arr[0x1E] = 'a';
    g_symbol_arr[0x1F] = 's'; g_symbol_arr[0x20] = 'd'; g_symbol_arr[0x21] = 'f';
    g_symbol_arr[0x22] = 'g'; g_symbol_arr[0x23] = 'h'; g_symbol_arr[0x24] = 'j';
    g_symbol_arr[0x25] = 'k'; g_symbol_arr[0x26] = 'l'; g_symbol_arr[0x27] = ';';
    g_symbol_arr[0x28] = '\''; g_symbol_arr[0x29] = '~'; g_symbol_arr[0x2C] = 'z';
    g_symbol_arr[0x2D] = 'x'; g_symbol_arr[0x2E] = 'c'; g_symbol_arr[0x2F] = 'v';
    g_symbol_arr[0x30] = 'b'; g_symbol_arr[0x31] = 'n'; g_symbol_arr[0x32] = 'm';
    g_symbol_arr[0x33] = ','; g_symbol_arr[0x34] = '.'; g_symbol_arr[0xF]= '\t';
    g_symbol_arr[0x35] = '/';g_symbol_arr[0x39] = ' '; [0x49] = PAGEUP_SYMBOL;
  [0x51] = PAGEDOWN_SYMBOL; [0x47] = HOME_SYMBOL; [0x4F] = END_SYMBOL;
  [0x52] = INSERT_SYMBOL; [0x53] = DELETE_SYMBOL; [0x48] = UP_SYMBOL; [0x50] = DOWN_SYMBOL;
  [0x4B] = LEFT_SYMBOL; [0x4D] = RIGHT_SYMBOL;
}

uint8_t patse_to_ascii(uint8_t input_symbol)
{
    return g_symbol_arr(input_symbol);
}

void keyboard_handler()
{
    uint8_t ascii_input = 0;
    // getting symbol from pic 
    uint8_t input_symbol = inb(KEYBOARD_INPUT_PORT);
    //if key pressed 
    if(input_symbol < 0x80)
    {
        ascii_input  = patse_to_ascii(input_symbol);
    }
    
    // sending ack to pic 
    outb(KEYBOARD_OUTPUT_PORT, 0X20);
}
