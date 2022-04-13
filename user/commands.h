#ifndef COMMANDS_H
#define COMMANDS_H

#define FUN_NAME_SIZE 11
#define FUN_INFO_SIZE 400
#define FUNS_NUM 16

typedef void (*command)(char* argv, int argc);

/*
	get string argument from argv
	@param argv: argument values
	@param argc: number of arguments
	@param argNum: requested argument
	@returns pointer to requested argument
*/

void init_commands();

void echo(char* argv, int argc);

void color(char* argv, int argc);

void help(char* argv, int argc);

void shutdownCommand(char* argv, int argc);

void bc(char* argv, int argc);

void unknown_command(char* argv, int argc);

void grep(char* argv, int argc);

void ls(char* argv, int argc);
void pwd(char* argv, int argc);
void cd(char* argv, int argc);
void cat(char* argv, int argc);
void rm(char* argv, int argc);
void touch(char* argv, int argc);
void mkdir(char* argv, int argc);

void test(char* argv, int argc);
void exec(char* argv, int argc);

void net(char* argv, int argc);


// After functions are defined:
static const char fun_names[FUNS_NUM][FUN_NAME_SIZE] = 
{
	"grep",
	"echo",
	"help",
	"color",
	"shutdown",
	"bc",
	"ls",
	"pwd",
	"cd",
	"cat",
	"rm",
	"touch",
	"mkdir",
	"test",
	"exec",
	"net"
};

static const command commands[FUNS_NUM] = 
{
	grep,
	echo,
	help,
	color,
	shutdownCommand,
	bc,
	ls,
	pwd,
	cd,
	cat,
	rm,
	touch,
	mkdir,
	test,
	exec,
	net
};

static const char fun_info[] = 
{
	"grep - Finds the requested string and prints it\n"
	"Usage: \'grep <arg1> <argv2>"

	"\0"
	
	"echo - Print message to the screen.\n"
	"Usage: \'echo <arg1> <arg2> ...\'\n"

	"\0"
	
	"help - Print information about available commands.\n"
	"Usage:\n"
	"\'help\' - Print all commands.\n"
	"\'help <command name>\' - Print information about a specific command.\n"

	"\0"
	
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
	"\t7 Light Gray\t15 White\n"

	"\0"
	
	"shutdown - shutdown the computer.\n"
	"Usage: \'shutdown\'\n"

	"\0"

	"bc - basic calculator.\n"
	"Usage: \'bc <expression>\'\n"
	"Only +,-,/,* and integers are allowed.\n"

	"\0"

	"ls - list items in folder\n"
	"Usage: \'ls <path - optional>\'\n"
	
	"\0"

	"pwd - print working directory\n"
	"Usage: \'pwd\'\n"

	"\0"

	"cd - change working directory\n"
	"Usage: \'cd <path>\'\n"

	"\0"

	"cat - print file content\n"
	"Usage: \'cat <path>\'\n"

	"\0"

	"rm - remove file\n"
	"Usage: \'rm <path>\'\n"


	"\0"

	"touch - create file\n"
	"Usage: \'touch <path>\'\n"

	"\0"

	"mkdir - create folder\n"
	"Usage: \'mkdir <path>\'\n"

	"\0"

	"test - test several key features of the OS.\n"
	"Usage: \'test <option>\'\n"
	"Options:\n"
	"i - test interrupts\n"
	"u - test usermode\n"
	"p - test paging\n"

	"\0"

	"exec - run binary file.\n"
	"Usage: \'exec <path>\'\n"

	"\0"

	"net - perform network commands.\n"
	"Usage:\n\'net info\' - print network info\n"
	"\'net arp <ip>\' - get mac address of ip\n"
	"\'net send <ip> <msg>\' - send message to another machine\n"
};

#endif