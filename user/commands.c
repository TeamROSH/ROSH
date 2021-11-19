#include "commands.h"
#include "../libc/screen.h"
#include "../libc/string.h"
#define NULL 0

char* getArg(char* argv, int argc, int argNum)
{
	if (argNum >= argc)		// prevent buffer overflow
		return NULL;
	char* res = argv;
	for (int i = 0; i < argNum; i++)		// run until wanted argument reached
	{
		res += strlen(res) + 1;		// next argument
	}
	return res;
}

void echo(char* argv, int argc)
{
	for (int i = 0; i < argc; i++)		// for every argument
	{
		puts(getArg(argv, argc, i));		// print it
		putc(' ');		// add the space
	}
}

void unknown_command(char* argv, int argc)
{
	puts("Command \'");
	puts(argv);
	puts("\' not found. Try \'help\'.");
}