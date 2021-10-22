#include "screen.h"

int cursor = 0;

void putc(char c)
{
	
}

/*
	get pointer of screen cursor
	Output: pointer of char
*/
char* getaddr()
{
	return (char*)SCREEN + 2 * cursor;		// get pointer by cursor
}