#ifndef PORTSH
#define PORTSH

#include <stdint.h>

/*
	read a byte from port (assembly in)
*/
unsigned char inb(uint16_t port);

/*
	write a byte to port (assembly out)
*/
void outb(uint16_t port, uint8_t data);

/*
	read a word from port (assembly in)
*/
unsigned short inw(uint16_t port);

/*
	write a word to port (assembly out)
*/
void outw(uint16_t port, uint16_t data);


/*
	read a dword from port (assembly in)
*/
uint32_t indw(uint16_t port);

/*
	writes a dword to port (assembly out)
*/
void outdw(uint16_t port, uint32_t data);
#endif