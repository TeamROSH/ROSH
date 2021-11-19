#include "user_main.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/screen.h"
#include "../libc/string.h"
#define INPUT_SIZE 41
#define OUTPUT_SIZE 240
#define FUN_NAME_SIZE 11
#define FUNS_NUM 4
#define NULL 0

char* getArg(char* argv, int argc, int argNum);
void callCommand(char* argv, int argc, char* output);

char fun_names[FUNS_NUM][FUN_NAME_SIZE] = 
{
	"grep",
	"echo",
	"help",
	"color"
}

void umain()
{
	char input[INPUT_SIZE] = {0};
	char output[OUTPUT_SIZE] = {0};
	while (1)
	{
		puts("\n$ ");
		getline(input, INPUT_SIZE);		// get input
		cmd(input, output);				// get output
		puts(output);			// print output
	}
}

void cmd(char* input, char* output)
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
}

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

/*
	call correct command from list
	@param argv: argument values
	@param argc: number of arguments
	@param output: pointer to output string
*/
void callCommand(char* argv, int argc, char* output)
{
	for (int i = 0; i < FUNS_NUM; i++)		// search functions
	{
		if (strncmp(argv, fun_names[i], strlen(fun_names[i])) == 0)		// if found
			funs[i](argv, argc, output);		// call it
	}
	unknown_command(output);		// print undefined command message
}