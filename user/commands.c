#include "commands.h"
#include "../libc/screen.h"

/*
	get string argument from argv
	@param argv: argument values
	@param argc: number of arguments
	@param argNum: requested argument
	@returns pointer to requested argument
*/
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
		putc(" ");		// add the space
	}
}