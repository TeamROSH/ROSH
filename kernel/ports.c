#include "ports.h"

/*
	read a byte from port (assembly in)
*/
uint8_t inb(uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/*
	write a byte to port (assembly out)
*/
void outb(uint16_t port, uint8_t data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

/*
	read a word from port (assembly in)
*/
uint16_t inw(uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/*
	write a word to port (assembly out)
*/
void outw(uint16_t port, uint16_t data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}