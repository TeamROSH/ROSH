#include "user_main.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/screen.h"
#include "../libc/string.h"
#define INPUT_SIZE 41
#define OUTPUT_SIZE 240
#define FUN_NAME_SIZE 11
#define NULL 0

char* getArg(char* argv, int argc, int argNum);

char fun_names[][FUN_NAME_SIZE] = 
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