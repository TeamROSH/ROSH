#include "user_main.h"
#include "commands.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/screen.h"
#include "../libc/string.h"
#define INPUT_SIZE 41
#define FUN_NAME_SIZE 11
#define FUNS_NUM 4
#define NULL 0

char* getArg(char* argv, int argc, int argNum);
void callCommand(char* argv, int argc);

char fun_names[FUNS_NUM][FUN_NAME_SIZE] = 
{
	"grep",
	"echo",
	"help",
	"color"
};



void umain()
{
	char input[INPUT_SIZE] = {0};
	while (1)
	{
		puts("\n$ ");
		getline(input, INPUT_SIZE);		// get input
		cmd(input);				// get output
	}
}

void cmd(char* input)
{
	int count = 0;
	for (int i = 0; i < strlen(input); i++)		// replace space with 0
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
			funs[i](getArg(argv, argc, 1), argc);		// call it
	}
	unknown_command(argv, argc);		// print undefined command message
}