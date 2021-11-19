#include "user_main.h"
#include "commands.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/screen.h"
#include "../libc/string.h"
#define INPUT_SIZE 41
#define FUN_NAME_SIZE 11
#define FUNS_NUM 4

void callCommand(char* argv, int argc);

char fun_names[FUNS_NUM][FUN_NAME_SIZE] = 
{
	"grep",
	"echo",
	"help",
	"color"
};

command commands[FUNS_NUM] = 
{
	unknown_command,
	echo,
	unknown_command,
	unknown_command
};

void umain()
{
	char input[INPUT_SIZE] = {0};

	while (1)
	{
		puts("\n\n$ ");
		bflush();		// clear buffer
		getline(input, INPUT_SIZE);		// get input
		cmd(input);				// get output
	}
}

void cmd(char* input)
{
	int count = 0;
	int size = strlen(input);
	for (int i = 0; i < size; i++)		// replace space with 0
	{
		if (input[i] == ' ')
		{
			input[i] = 0;
			count++;
		}
	}
	callCommand(input, count);		// call function
}

/*
	call correct command from list
	@param argv: argument values
	@param argc: number of arguments
*/
void callCommand(char* argv, int argc)
{
	for (int i = 0; i < FUNS_NUM; i++)		// search functions
	{
		if (strncmp(argv, fun_names[i], strlen(fun_names[i])) == 0)		// if found
		{
			commands[i](getArg(argv, argc, 1), argc);		// call it
			return;
		}
	}
	unknown_command(argv, argc);		// print undefined command message
}