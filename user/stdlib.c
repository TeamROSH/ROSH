#include "stdlib.h"

void ushutdown(void)
{
	syscall(G_SYSTEM, F_SHUTDOWN, 0, 0);
}

void uputc(char c)
{
	uint32_t params[1];
	params[0] = (uint32_t)c;
	syscall(G_OUTPUT, F_PUTC, params, 1);
}

void uputs(const char* str)
{
	uint32_t params[1];
	params[0] = (uint32_t)str;
	syscall(G_OUTPUT, F_PUTS, params, 1);
}

void uputi(int n)
{
	uint32_t params[1];
	params[0] = (uint32_t)n;
	syscall(G_OUTPUT, F_PUTI, params, 1);
}

void usetColor(char fore, char back)
{
	uint32_t params[2];
	params[0] = (uint32_t)fore;
	params[1] = (uint32_t)back;
	syscall(G_OUTPUT, F_COLOR, params, 2);
}

char ugetchar()
{
	char c = 0;
	uint32_t params[1];
	params[0] = (uint32_t)&c;
	syscall(G_INPUT, F_GETCHAR, params, 1);
	return c;
}

void ubflush()
{
	syscall(G_INPUT, F_BFLUSH, 0, 0);
}

void ugetline(char* pStr, int size)
{
	uint32_t params[2];
	params[0] = (uint32_t)&pStr;
	params[1] = (uint32_t)size;
	syscall(G_INPUT, F_GETLINE, params, 2);
}