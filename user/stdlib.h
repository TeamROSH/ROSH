#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "../libc/memory.h"
#include "../libc/string.h"
#include "syscalls/syscalls.h"
#include "heap.h"

/*
	Shutdown the system
*/
void ushutdown(void);

/*
	print a char to the screen
	@param c: char to print
*/
void uputc(char c);

/*
	print a string to the screen
	@param str: string to print
*/
void uputs(const char* str);

/*
	print a number to the screen
	@param n: number to print
*/
void uputi(int n);

/*
	set the screen color
	@param fore: foreground color code
	@param back: background color code
*/
void usetColor(char fore, char back);

/*
	get char from input buffer
	@returns input char
*/
char ugetchar();

/*
	Flush buffer
*/
void ubflush();

/*
	get line ends with \n from buffer
	@param pStr: pointer to returned string
	@param size: size of requested string
*/
void ugetline(char* pStr, int size);

#endif