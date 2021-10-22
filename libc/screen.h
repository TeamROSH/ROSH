#ifndef SCREENH
#define SCREENH

#define SCREEN 0xb8000
#define ROWS 25
#define COLS 80
#define print(value) _Generic(value, int: puti, char*: puts, char: putc)(value)

/*
	print a char to the screen
	Input: the char to print
*/
void putc(char c);

/*
	print a string to the screen
	Input: the string to print (ends with \0)
*/
void puts(char* str);

/*
	print a number to the screen
	Input: the number to print
*/
void puti(int n);

#endif