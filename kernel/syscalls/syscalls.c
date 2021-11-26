#include "syscalls.h"

void syscall(uint16_t group, uint16_t function, uint32_t* params, int n)
{
	uint32_t id = group;		// calculate function number
	id <<= 16;
	id += function;

	asm volatile
	(
		"mov %0, %%eax;"
		"mov %1, %%esi;"
		"mov %2, %%edi;"
		"int $0x80;"
		: : "r" (id), "r" (params), "r" (n)
		: "eax", "esi", "edi"
	);
}

void syscall_handler(registers_t* registers)
{
	uint32_t id = registers->eax;		// get function id
	uint16_t group = (uint16_t)(id >> 16);
	uint16_t function = (uint16_t)id;

	
}