#include "screen.h"

int cursor = 0;

char* getaddr();

void putc(char c)
{
	if (cursor < ROWS * COLS)		// if screen not full
	{
		char* pos = getaddr();		// get pos of char
		pos[0] = c;		// print char
		cursor++;		// add to cursor
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