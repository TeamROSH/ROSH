#include "commands.h"
#include "../libc/screen.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/string.h"
#include "../libc/system.h"
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

void unknown_command(char* argv, int argc)
{
	puts("Command \'");
	puts(argv);
	puts("\' not found. Try \'help\'.");
}

void echo(char* argv, int argc)
{
	for (int i = 1; i < argc; i++)		// for every argument except command name
	{
		puts(getArg(argv, argc, i));		// print it
		putc(' ');		// add the space
	}
}

void color(char* argv, int argc)
{
	if (argc != 3)
		puts("Invalid syntax. Try \'help color\'.");
	else
	{
		char fore = (char)atoi(getArg(argv, argc, 1));		// get foreground color
		char back = (char)atoi(getArg(argv, argc, 2));		// get background color
		setScreenColor(fore, back);				// set screen colors
	}
}

void help(char* argv, int argc)
{
	if (argc == 1)		// no specific command
	{
		puts(
			"For specific command:\n"
			"help <command name>\n\n"
			"Available Commands:\n"
			"help - Print information about available commands.\n"
			"echo - Print message to the screen.\n"
			"color - Change the shell's colors.\n"
			"shutdown - shutdown the computer.\n"
		);
	}
	else if (argc == 2)		// for specific command
	{
		for (int i = 0; i < FUNS_NUM; i++)		// search functions
		{
			if (strncmp(getArg(argv, argc, 1), fun_names[i], strlen(fun_names[i])) == 0)		// if found
			{
				puts(fun_info[i]);		// print info
			}
		}
	}
	else
	{
		puts("Invalid syntax. Try \'help help\'.");
	}
}

void shutdownCommand(char* argv, int argc)
{
	puts("Are you sure? (y/n): ");
	bflush();
	char res = getchar();
	if (res == 'y')
		shutdown();
}