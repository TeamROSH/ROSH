#include "syscalls.h"
#include "../../libc/system.h"
#include "../../libc/screen.h"
#include "../IDT/keyboard.h"
#include "../memory/heap.h"

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
			if (n == 0)
			{
				shutdown();
			}
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
		else if (function == F_PUTI)
		{
			if (n == 1)
			{
				puti((int)params[0]);
			}
		}
		else if (function == F_COLOR)
		{
			if (n == 2)
			{
				setScreenColor((char)(params[0]), (char)(params[1]));
			}
		}
	}
	else if (group == G_INPUT)
	{
		if (function == F_GETCHAR)
		{
			if (n == 1)
			{
				asm volatile("sti");		// enable interrupts
				*((char*)(params[0])) = getchar();
				asm volatile("cli");		// disable interrupts
			}
		}
		else if (function == F_GETLINE)
		{
			if (n == 2)
			{
				asm volatile("sti");		// enable interrupts
				getline(*((char**)(params[0])), (int)(params[1]));
				asm volatile("cli");		// disable interrupts
			}
		}
		else if (function == F_BFLUSH)
		{
			if (n == 0)
			{
				bflush();
			}
		}
	}
	else if (group == G_MEMORY)
	{
		if (function == F_MALLOC)
		{
			if (n == 2)
			{
				*((void**)(params[0])) = k_umalloc((int)(params[1]));
			}
		}
		else if (function == F_REALLOC)
		{
			if (n == 3)
			{
				*((void**)(params[0])) = k_urealloc((void*)params[1],(int)(params[2]));
			}
		}
		else if (function == F_FREE)
		{
			if (n == 1)
			{
				k_ufree((void*)params[0]);
			}
		}
	}
}