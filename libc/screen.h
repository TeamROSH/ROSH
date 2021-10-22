#ifndef SCREENH
#define SCREENH

#define SCREEN 0xb8000
#define ROWS 25
#define COLS 80

/*
	print a char to the screen
	Input: the char to print
*/
void putc(char c);

/*
	print a string to the screen
	Input: the strint to print (ends with \0)
*/
void puts(char* str);

#endif