#include "screen.h"
#include "string.h"
#include "../kernel/ports.h"
#include <stdint.h>
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define TAB 4

int cursor = 0;

char* getaddr();
void moveCursor(int n);

void putc(char c)
{
	if (cursor < ROWS * COLS)		// if screen not full
	{
		if (c != '\n' && c != '\t' && c != '\r')
		{
			char* pos = getaddr();		// get pos of char
			pos[0] = c;		// print char
			moveCursor(1);
		}
		else if (c == '\n')
		{
			if (cursor / COLS + 1 < ROWS)
				moveCursor(COLS - (cursor % COLS));		// set cursor to start of next line
			else
			{
				// scroll option
			}
		}
		else if (c == '\t')
		{
			if (cursor < ROWS * COLS - TAB)		// if room of tab
				moveCursor(TAB);
			else
			{
				// scroll option
			}
		}
		else if (c == '\r')
		{
			moveCursor(-(cursor % COLS));		// move to the start of the line
		}
	}
	else
	{
		// scroll option
	}
}

/*
	get pointer of screen cursor
	Output: pointer of char
*/
char* getaddr()
{
	return (char*)SCREEN + 2 * cursor;		// get pointer by cursor
}

/*
	move cursor n steps
	Input: n - number of steps to move
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