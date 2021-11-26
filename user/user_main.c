#include "user_main.h"
#include "commands.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/screen.h"
#include "../libc/string.h"
#include "../kernel/syscalls/syscalls.h"
#define INPUT_SIZE 41

void callCommand(char* argv, int argc);

void umain()
{
	char input[INPUT_SIZE] = {0};

	char a = 'a';
	syscall(1, 0, (uint32_t*)(&a), 1);

	while (1)
	{
		//puts("\n\n$ ");
		//bflush();		// clear buffer
		//getline(input, INPUT_SIZE);		// get input
		//cmd(input);				// get output
	}
}

void cmd(char* input)
{
	int count = 1;		// including command name
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
			commands[i](argv, argc);		// call it
			return;
		}
	}
	unknown_command(argv, argc);		// print undefined command message
}