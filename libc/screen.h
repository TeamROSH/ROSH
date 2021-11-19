#ifndef SCREENH
#define SCREENH

#include <stdint.h>
#define SCREEN 0xb8000
#define ROWS 25
#define COLS 80

/*
	print a char to the screen
	@param c: char to print
*/
void putc(char c);

/*
	print a string to the screen
	@param str: string to print
*/
void puts(char* str);

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

#endif