#ifndef COMMANDS_H
#define COMMANDS_H

/*
	get string argument from argv
	@param argv: argument values
	@param argc: number of arguments
	@param argNum: requested argument
	@returns pointer to requested argument
*/
char* getArg(char* argv, int argc, int argNum);


typedef void (*command)(char* argv, int argc);

void echo(char* argv, int argc);

void unknown_command(char* argv, int argc);

#endif