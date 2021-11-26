#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include "../IDT/time.h"

/*
	Syscaller for usermode
	@param group: group of functions
	@param function: function number in the group
	@param params: parameters to pass to the system call
	@param n: number of parameters
*/
void syscall(uint16_t group, uint16_t function, uint32_t* params, int n);

void syscall_handler(registers_t* registers);

#endif