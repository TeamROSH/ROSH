#include "screen.h"
#include <stdint.h>
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

int cursor = 0;

char* getaddr();
void moveCursor(int n);

void putc(char c)
{
	if (cursor < ROWS * COLS)		// if screen not full
	{
		char* pos = getaddr();		// get pos of char
		pos[0] = c;		// print char
		moveCursor(1);
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