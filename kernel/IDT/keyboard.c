#include "keyboard.h"
#include "../../libc/memory.h"
#define BUFFER_SIZE 100
#define FALSE 0
#define TRUE !FALSE

uint8_t g_symbol_arr[NUM_OF_SYMBOLS];
uint8_t g_keyboard_input[NUM_OF_SYMBOLS];
char buffer[100] = {0};
int buffer_size = 0;
int special = FALSE;
int pending = FALSE;
int enterPress = FALSE;

void keyboard_handler();
uint8_t symbol_to_ascii(uint8_t input_symbol);
void keyboard_initialize();
int keyboard_putc(uint8_t input_char);
char pop_buffer();

void keyboard_initialize()
{
    for(int i = 0; i < NUM_OF_SYMBOLS; i++)
    {
        g_symbol_arr[i] = UNKNOWN_SYMBOL;
        g_keyboard_input[i] = UNKNOWN_SYMBOL;
    }

    g_symbol_arr[0x02] = '1'; g_symbol_arr[0x03] = '2';
	g_symbol_arr[0x04] = '3'; g_symbol_arr[0x05] = '4'; g_symbol_arr[0x06] = '5'; g_symbol_arr[0x07] = '6';
	g_symbol_arr[0x08] = '7'; g_symbol_arr[0x09] = '8'; g_symbol_arr[0x0A] = '9'; g_symbol_arr[0x0B] = '0';
	g_symbol_arr[0x0C] = '-'; g_symbol_arr[0x0D] = '='; g_symbol_arr[0x0E] = '\b'; g_symbol_arr[0x0F] = '\t';
	g_symbol_arr[0x10] = 'q'; g_symbol_arr[0x11] = 'w'; g_symbol_arr[0x12] = 'e'; g_symbol_arr[0x13] = 'r';
	g_symbol_arr[0x14] = 't'; g_symbol_arr[0x15] = 'y'; g_symbol_arr[0x16] = 'u'; g_symbol_arr[0x17] = 'i';
	g_symbol_arr[0x18] = 'o'; g_symbol_arr[0x19] = 'p'; g_symbol_arr[0x1A] = '['; g_symbol_arr[0x1B] = ']';
	g_symbol_arr[0x1C] = '\n'; g_symbol_arr[0x1D] = CTRL_PRESS; g_symbol_arr[0x1E] = 'a'; g_symbol_arr[0x1F] = 's';
	g_symbol_arr[0x20] = 'd'; g_symbol_arr[0x21] = 'f'; g_symbol_arr[0x22] = 'g'; g_symbol_arr[0x23] = 'h';
	g_symbol_arr[0x24] = 'j'; g_symbol_arr[0x25] = 'k'; g_symbol_arr[0x26] = 'l'; g_symbol_arr[0x27] = ';';
	g_symbol_arr[0x28] = '\''; g_symbol_arr[0x29] = '`'; g_symbol_arr[0x2A] = SHIFT_PRESS; g_symbol_arr[0x2B] = '\\';
	g_symbol_arr[0x2C] = 'z'; g_symbol_arr[0x2D] = 'x'; g_symbol_arr[0x2E] = 'c'; g_symbol_arr[0x2F] = 'v';
	g_symbol_arr[0x30] = 'b'; g_symbol_arr[0x31] = 'n'; g_symbol_arr[0x32] = 'm'; g_symbol_arr[0x33] = ',';
	g_symbol_arr[0x34] = '.'; g_symbol_arr[0x35] = '/'; g_symbol_arr[0x36] = UNKNOWN_SYMBOL; // rshift
	g_symbol_arr[0x37] = '*';
	g_symbol_arr[0x38] = ALT_PRESS; g_symbol_arr[0x39] = ' '; g_symbol_arr[0x3A] = CAPSLOCK_PRESS;
	g_symbol_arr[0x3B] = '1'; g_symbol_arr[0x3C] = '2'; g_symbol_arr[0x3D] = '3'; g_symbol_arr[0x3E] = '4';
	g_symbol_arr[0x3F] = '5'; g_symbol_arr[0x40] = '6'; g_symbol_arr[0x41] = '7'; g_symbol_arr[0x42] = '8';
	g_symbol_arr[0x43] = '9'; g_symbol_arr[0x44] = '1'; g_symbol_arr[0x45] = '0'; g_symbol_arr[0x57] = '1'; // F(num), 0 is numlock
	g_symbol_arr[0x46] = UNKNOWN_SYMBOL;	// scrolllock
	g_symbol_arr[0x47] = '7';
	g_symbol_arr[0x48] = '8'; g_symbol_arr[0x49] = '9'; g_symbol_arr[0x4A] = '-'; g_symbol_arr[0x4B] = '4';
	g_symbol_arr[0x4C] = '5'; g_symbol_arr[0x4D] = '6'; g_symbol_arr[0x4E] = '+'; g_symbol_arr[0x4F] = '1';
	g_symbol_arr[0x50] = '2'; g_symbol_arr[0x51] = '3'; g_symbol_arr[0x52] = '0'; g_symbol_arr[0x53] = '.';	// from keypad
	g_symbol_arr[0x49] = PAGEUP_SYMBOL;
    g_symbol_arr[0x51] = PAGEDOWN_SYMBOL; g_symbol_arr[0x47] = HOME_SYMBOL; g_symbol_arr[0x4F] = END_SYMBOL;
    g_symbol_arr[0x52] = INSERT_SYMBOL; g_symbol_arr[0x53] = DELETE_SYMBOL; g_symbol_arr[0x48] = UP_SYMBOL; 
    g_symbol_arr[0x50] = DOWN_SYMBOL; g_symbol_arr[0x4B] = LEFT_SYMBOL; g_symbol_arr[0x4D] = RIGHT_SYMBOL;
	g_symbol_arr[0x9D] = CTRL_RELEASE; g_symbol_arr[0xAA] = SHIFT_RELEASE; g_symbol_arr[0xB8] = ALT_RELEASE;
}

uint8_t symbol_to_ascii(uint8_t input_symbol)
{
    return g_symbol_arr[input_symbol];
}

int keyboard_putc(uint8_t input_char)
{
    if(input_char != 0x1D && input_char != 0x2A && input_char != 0x38 && 
		input_char != 0x3A && input_char < 0x40)
    {
		char realValue = (char)symbol_to_ascii(input_char);
		if (pending)
		{
			if (realValue == '\b')		// if bs remove one
			{
				if (buffer_size > 0)
				{
					buffer_size--;
					putc(realValue);
				}
			}
			else if (realValue == '\n')
			{
				enterPress = TRUE;
				putc(realValue);
			}
			else if (buffer_size < BUFFER_SIZE)				// if char add one to buffer
			{
				buffer[buffer_size] = key_replacement(realValue);
				buffer_size++;
				putc(realValue);
			}
		}
		else
		{
			putc(realValue);
		}
        return TRUE;
    }
	else if (special || input_char == 0x1D || input_char == 0x2A || input_char == 0x38 || 
		input_char == 0x3A || input_char >= 0x9D){		// if special or one of control keys
		non_char_print((char)symbol_to_ascii(input_char));
	}
    return FALSE;
}

void keyboard_handler(registers_t* registers)
{
    uint8_t ascii_input = 0;
    // getting symbol from pic 
    uint8_t input_symbol = inb(KEYBOARD_INPUT_PORT);
    //if key pressed 
    if(input_symbol != 0)
    {
        ascii_input  = symbol_to_ascii((uint8_t)input_symbol);
        keyboard_putc((uint8_t)input_symbol);
    }
    
    // sending ack to pic 
    outb(KEYBOARD_OUTPUT_PORT, 0X20);
}

void allowSpecial(int allow)
{
	special = allow;
}

void bflush()
{
	buffer_size = 0;
}

/*
	pop first buffer char
	@returns first buffer char
*/
char pop_buffer()
{
	char val = 0;
	if (buffer_size > 0)
	{
		val = buffer[0];		// get char
		buffer_size--;
		memcpy(buffer, buffer + 1, buffer_size);		// copy back
	}
	return val;
}

char getchar()
{
	pending = TRUE;
	while (!enterPress){}		// wait until enter
	enterPress = FALSE;
	pending = FALSE;
	return pop_buffer();
}

void getline(char* pStr, int size)
{
	pending = TRUE;
	while (!enterPress){}		// wait until enter
	for (int i = 0; i < size - 1; i++)
		pStr[i] = pop_buffer();
	pStr[size - 1] = 0;
	enterPress = FALSE;
	pending = FALSE;
}