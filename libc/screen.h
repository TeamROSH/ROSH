#ifndef SCREENH
#define SCREENH

#include <stdint.h>
#include "../kernel/memory/memorylayout.h"

#define SCREEN VIDEO_MEM_START		// VIDEO_MEM_PHYSICAL_ADDR
#define ROWS 25
#define COLS 80

#define hex(a) 48 + (a / 10) * 49 + (a % 10)

/*
	print a char to the screen
	@param c: char to print
*/
void putc(char c);

/*
	print a string to the screen
	@param str: string to print
*/
void puts(const char* str);

/*
	print a number to the screen
	@param n: number to print
*/
void puti(int n);

/*
	init the console cursor
*/
void initConsole();

/*
	print non char (Arrows, etc.)
	@param c: the character
*/
void non_char_print(uint8_t c);

/*
	clear the console
*/
void clearConsole();

/*
	screenTrackerUp getter
	@returns screenTrackerUp
*/
char* getTrackerUp();

/*
	screenTrackerDown getter
	@returns screenTrackerDown
*/
char* getTrackerDown();

/*
	set the screen color
	@param fore: foreground color code
	@param back: background color code
*/
void setScreenColor(char fore, char back);

#endif