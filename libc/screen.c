#include "screen.h"
#include "string.h"
#include "../kernel/ports.h"
#include "../kernel/IDT/keyboard.h"
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define TAB 4
#define FALSE 0
#define TRUE !FALSE
#define bool char

char* getaddr();
void moveCursor(int n);
int get_cursor_position(void);
int print_special(char c);
int printable(char c);
void up_putc(char c);

int cursor = 0;
bool key_flags[] = {FALSE, FALSE, FALSE, FALSE};		// ctrl shift alt CapsLock

void putc(char c)
{
	if (!print_special(c) && cursor < ROWS * COLS)
	{
		up_putc(c);
	}
	else if (cursor == ROWS * COLS)
	{
		// scroll option
	}
}

/*
	get pointer of screen cursor
	@returns pointer of char
*/
char* getaddr()
{
	return (char*)SCREEN + 2 * cursor;		// get pointer by cursor
}

/*
	move cursor n steps
	@param n: number of steps to move
*/
void moveCursor(int n)
{
	cursor += n;
	outb(REG_SCREEN_CTRL, 15);		// update cursor on screen
    outb(REG_SCREEN_DATA, (uint8_t)(cursor & 0xff));
	outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (uint8_t)((cursor >> 8) & 0xff));
}

void puts(char* str)
{
	int len = strlen(str);		// get string length
	for (int i = 0; i < len; i++)		// for every char in string
	{
		putc(str[i]);		// print it
	}
}

void puti(int n)
{
	int size = digits(n), temp = 0;
	for (int i = 0; i < size; i++)		// for every digit
	{
		temp = n;
		for (int j = 0; j < size - i - 1; j++)		// get digit
			temp /= 10;
		putc((char)(temp % 10 + ASCII_NUM_OFFSET));		// print it
	}
}

int get_cursor_position(void)
{
    int pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((int)inb(0x3D5)) << 8;
    return pos;
}

void initConsole()
{
	cursor = get_cursor_position();
}

/*
	print a special character
	@param c: the char to print
	@returns was special character printed
*/
int print_special(char c)
{
	switch (c)
	{
		case '\n':
			if (cursor / COLS + 1 < ROWS)
				moveCursor(COLS - (cursor % COLS));		// set cursor to start of next line
			else
			{
				// scroll option
			}
			return TRUE;
		case '\t':
			if (cursor < ROWS * COLS - TAB)		// if room of tab
				moveCursor(TAB);
			else
			{
				// scroll option
			}
			return TRUE;
		case '\r':
			moveCursor(-(cursor % COLS));		// move to the start of the line
			return TRUE;
		case '\b':
			if (cursor > 0)
			{
				moveCursor(-1);		// delete last insert
				up_putc('\0');
				moveCursor(-1);
			}
			return TRUE;
		default:
			return !printable(c);
	}
}

/*
	return if char is printable
	@param c: the char to print
	@returns is printable
*/
int printable(char c)
{
	return c >= 32 && c <= 126;		// between space and ~ in ascii table
}

/*
	print a char to the screen (private unprotected version)
	@param c: char to print
*/
void up_putc(char c)
{
	char* pos = getaddr();		// get pos of char
	pos[0] = c;		// print char
	moveCursor(1);
}

void non_char_print(uint8_t c)
{
	switch (c)
	{
		case PAGEUP_SYMBOL:
			moveCursor(-cursor);		// bring to the top
			break;
		case PAGEDOWN_SYMBOL:
			moveCursor((ROWS - 1) * COLS - cursor);		// bring to last line
			break;
		case HOME_SYMBOL:
			moveCursor(-(cursor % COLS));		// move to the start of the line
			break;
		case END_SYMBOL:
			moveCursor(COLS - (cursor % COLS) - 1);		// set cursor to end of line
			break;
		case INSERT_SYMBOL:
			break;
		case DELETE_SYMBOL:
			if (cursor < ROWS * COLS - 1){
				up_putc('\0');
				moveCursor(-1);
			}
			break;
		case UP_SYMBOL:
			if (cursor >= COLS)
				moveCursor(-COLS);
			break;
		case DOWN_SYMBOL:
			if (cursor <= ((ROWS - 1) * COLS - 1))
				moveCursor(COLS);
			break;
		case LEFT_SYMBOL:
			if (cursor > 0)
				moveCursor(-1);
			break;
		case RIGHT_SYMBOL:
			if (cursor < ROWS * COLS - 1)
				moveCursor(1);
			break;
	}
}