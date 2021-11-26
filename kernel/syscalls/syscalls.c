#include "syscalls.h"
#include "../../libc/system.h"
#include "../../libc/screen.h"

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

	uint32_t* params = (uint32_t*)registers->esi;		// get parameters
	uint32_t n = registers->edi;

	if (group == G_SYSTEM)
	{
		if (function == F_SHUTDOWN)
		{
			shutdown();
		}
	}
	else if (group == G_OUTPUT)
	{
		if (function == F_PUTC)
		{
			if (n == 1)
			{
				putc((char)params[0]);
			}
		}
		else if (function == F_PUTS)
		{
			if (n == 1)
			{
				puts((char*)params[0]);
			}
		}
	}
}