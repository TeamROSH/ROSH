#ifndef USER_MAIN_H
#define USER_MAIN_H

#include "commands.h"
#include "stdlib.h"

/*
	Userspace main function
*/
void umain();

/*
	command handler
	@param input: input string
*/
void cmd(char* input);

#endif