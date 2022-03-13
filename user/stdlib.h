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

/*
	create a folder
*/
void ucreate_folder(char* path);

/*
	create a file
*/
void ucreate_file(char* path);

/*
	get file type
*/
int ufile_type(char* path);

/*
	delete a file
*/
void udelete_file(char* path);

/*
	read from file
	@returns number of bytes read
*/
int uread_file(char* path, char* res);

/*
	write data to file
*/
void uwrite_file(char* path, const char* data, int size, int append);

/*
	get file size in bytes
*/
int ufile_size(char* path);

/*
	create a new process
*/
void unew_process(char* path);

/*
	print net info
*/
void unet_info();

/*
	perform arp request
*/
void unet_arp(uint32_t ip);

/*
	perform rosh msg send
*/
void unet_send(uint32_t ip, char* msg);

/*
	sleep ms
*/
void usleep(uint32_t ms);

#endif