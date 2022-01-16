#include "syscalls.h"
#include "../../libc/system.h"
#include "../../libc/screen.h"
#include "../IDT/keyboard.h"
#include "../memory/heap.h"
#include "../../fs/fs.h"
#include "../process/process.h"

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
	else if(group == G_FS)
	{
		if (function == F_CREATE_F)
		{
			if (n == 1)
			{
				create_file((char*)params[0]);
			}
		}
		else if (function == F_CREATE_D)
		{
			if (n == 1)
			{
				create_folder((char*)params[0]);
			}
		}
		else if (function == F_FS_DELETE)
		{
			if (n == 1)
			{
				delete_file((char*)params[0]);
			}
		}
		else if (function == F_FS_SIZE)
		{
			if (n == 2)
			{
				*((int*)params[1]) = file_size((char*)params[0]);
			}
		}
		else if (function == F_FS_READ)
		{
			if (n == 3)
			{
				*((int*)params[2]) = read_file((char*)params[0], *((char**)(params[1])));
			}
		}
		else if (function == F_FS_WRITE)
		{
			if (n == 4)
			{
				write_file((char*)params[0], (char*)params[1], (int)params[2], (int)params[3]);
			}
		}
		else if (function == F_FS_TYPE)
		{
			if (n == 2)
			{
				*((int*)params[1]) = file_type((char*)params[0]);
			}
		}
	}
	else if (group == G_PROCESS)
	{
		if (function == F_NEW_PROC)
		{
			if (n == 1)
			{
				new_process((char*)params[0]);
			}
		}
	}
}