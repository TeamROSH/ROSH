#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>

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

#define G_MEMORY	3
	#define F_MALLOC	0
	#define F_REALLOC	1
	#define F_FREE		2

/*
	Syscaller for usermode (Ring 3)
	@param group: group of functions
	@param function: function number in the group
	@param params: parameters to pass to the system call
	@param n: number of parameters
*/
void syscall(uint16_t group, uint16_t function, uint32_t* params, int n);

#endif