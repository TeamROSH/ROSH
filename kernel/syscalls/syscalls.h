#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include "../IDT/reg_def.h"

#define G_SYSTEM	0
	#define F_SHUTDOWN	0

#define G_OUTPUT	1
	#define F_PUTC		0
	#define F_PUTS		1
	#define F_PUTI		2
	#define F_COLOR		3

#define G_INPUT		2
	#define F_GETCHAR	0
	#define F_GETLINE	1
	#define F_BFLUSH	2

/*
	Syscall handler (Ring 0)
*/
void syscall_handler(registers_t* registers);

#endif