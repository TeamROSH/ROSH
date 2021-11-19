#ifndef USER_MAIN_H
#define USER_MAIN_H

/*
	Userspace main function
*/
void umain();

/*
	command handler
	@param input: input string
	@param output: output pointer
*/
void cmd(char* input, char* output);

#endif