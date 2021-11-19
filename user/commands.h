#ifndef COMMANDS_H
#define COMMANDS_H

#define FUN_NAME_SIZE 11
#define FUN_INFO_SIZE 400
#define FUNS_NUM 5

typedef void (*command)(char* argv, int argc);

/*
	get string argument from argv
	@param argv: argument values
	@param argc: number of arguments
	@param argNum: requested argument
	@returns pointer to requested argument
*/
char* getArg(char* argv, int argc, int argNum);

void echo(char* argv, int argc);

void color(char* argv, int argc);

void help(char* argv, int argc);

void shutdownCommand(char* argv, int argc);

void unknown_command(char* argv, int argc);



// After functions are defined:
static const char fun_names[FUNS_NUM][FUN_NAME_SIZE] = 
{
	"grep",
	"echo",
	"help",
	"color",
	"shutdown"
};

static const command commands[FUNS_NUM] = 
{
	unknown_command,
	echo,
	help,
	color,
	shutdownCommand
};

static const char *fun_info[] = 
{
	"Not yet defined.\n",
	
	"echo - Print message to the screen.\n"
	"Usage: \'echo <arg1> <arg2> ...\'\n",
	
	"help - Print information about available commands.\n"
	"Usage:\n"
	"\'help\' - Print all commands.\n"
	"\'help <command name>\' - Print information about a specific command.\n",
	
	"color - Change the shell's colors.\n"
	"Usage: \'color <foreground> <background>\'\n\n"
	"Options:\n"
	"\t0 Black \t\t8  Dark Gray\n"
	"\t1 Blue  \t\t9  Light Blue\n"
	"\t2 Green \t\t10 Light Green\n"
	"\t3 Cyan  \t\t11 Light Cyan\n"
	"\t4 Red   \t\t12 Light Red\n"
	"\t5 Magenta   \t13 Light Magenta\n"
	"\t6 Brown \t\t14 Yellow\n"
	"\t7 Light Gray\t15 White\n",
	
	"shutdown - shutdown the computer.\n"
	"Usage: \'shutdown\'\n"
};

#endif