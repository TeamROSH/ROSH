#ifndef STDLIB_H
#define STDLIB_H

#include "../libc/memory.h"
#include "../libc/string.h"
#include "../kernel/syscalls/syscalls.h"

// Groups
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


#define G_MEMORY	3
	#define F_MALLOC	0
	#define F_REALLOC	1
	#define F_FREE		2

#endif