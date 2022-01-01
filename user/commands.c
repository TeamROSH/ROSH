#include "commands.h"
#include "stdlib.h"

#define NULL 0
#define FALSE 0
#define TRUE !FALSE

char dir[200] = {0};

void init_commands()
{
	dir[0] = '/';
}

void grep(char* argv, int argc)
{
	/*char *token = NULL;

	// validating length
	if(argc != 3)
	{
		puts("Invalid syntax. Try \'help grep\'.");
	}
	else
	{

		token = strtok(argv[2], "\n");
		
		// going through the lines in the string
		while(token != NULL)
		{
			// if string included
			if(strstr(token, argv[1]) == 0)
			{
				puts(token)
			}
			token = strtok(NULL, argv[2]);
		}
	}*/
}

void unknown_command(char* argv, int argc)
{
	uputs("Command \'");
	uputs(argv);
	uputs("\' not found. Try \'help\'.");
}

void echo(char* argv, int argc)
{
	if (argc >= 4 && getArg(argv, argc, argc - 2)[0] == '>' && strlen(getArg(argv, argc, argc - 2)) <= 2)
	{
		if (strlen(getArg(argv, argc, argc - 2)) == 2 && getArg(argv, argc, argc - 2)[1] != '>')
		{
			uputs("Invalid syntax. Try \'help echo\'.");
			return;
		}
		char path[200] = {0};
		memcpy(path, dir, strlen(dir));
		const char* rel = getArg(argv, argc, argc - 1);
		if (rel[0] == '/')
			memcpy(path, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(path + strlen_dir, rel, strlen(rel) + 1);
		}

		if (strlen(getArg(argv, argc, argc - 2)) == 2)
			uwrite_file(path, "\n", 1, 1);

		for (int i = 1; i < argc - 2; i++)		// for every argument except function arguments
		{
			uwrite_file(path, getArg(argv, argc, i), strlen(getArg(argv, argc, i)), !(strlen(getArg(argv, argc, argc - 2)) == 1 && i == 1));
			if (i != argc - 3)
				uwrite_file(path, " ", 1, 1);
		}
	}
	else
	{
		for (int i = 1; i < argc; i++)		// for every argument except command name
		{
			uputs(getArg(argv, argc, i));		// print it
			uputc(' ');		// add the space
		}
	}
}

void color(char* argv, int argc)
{
	if (argc != 3)
		uputs("Invalid syntax. Try \'help color\'.");
	else
	{
		char fore = (char)atoi(getArg(argv, argc, 1));		// get foreground color
		char back = (char)atoi(getArg(argv, argc, 2));		// get background color
		usetColor(fore, back);				// set screen colors
	}
}

void help(char* argv, int argc)
{
	if (argc == 1)		// no specific command
	{
		uputs(
			"For specific command:\n"
			"help <command name>\n\n"
			"Available Commands:\n"
			"help - Print information about available commands.\n"
			"echo - Print message to the screen.\n"
			"color - Change the shell's colors.\n"
			"grep - \n"
			"shutdown - shutdown the computer.\n"
			"bc - basic calculator.\n"
			"ls - list items in folder\n"
			"pwd - print working directory\n"
			"cd - change working directory\n"
			"cat - print file content\n"
			"rm - remove file or folder\n"
			"touch - create file\n"
			"mkdir - create folder\n"
		);
	}
	else if (argc == 2)		// for specific command
	{
		for (int i = 0; i < FUNS_NUM; i++)		// search functions
		{
			if (strncmp(getArg(argv, argc, 1), fun_names[i], strlen(fun_names[i])) == 0)		// if found
			{
				uputs(getArg(fun_info, FUNS_NUM, i));		// print info
				return;
			}
		}
		uputs("Unknown command. Try \'help\'.");
	}
	else
	{
		uputs("Invalid syntax. Try \'help help\'.");
	}
}

void shutdownCommand(char* argv, int argc)
{
	uputs("Are you sure? (y/n): ");
	ubflush();
	char res = ugetchar();
	if (res == 'y')
		ushutdown();
}

void bc(char* argv, int argc)
{
	// local functions
	int isMathOperator(char c)
	{
		return c == '*' || c == '/' || c == '+' || c == '-';
	}

	int isValidExp(char* exp, int size)			// check for only ints and +-*/
	{
		if (isMathOperator(exp[0]))
			return FALSE;
		if (isMathOperator(exp[size - 1]))
			return FALSE;
		for (int i = 0; i < size; i++)
			if (!((exp[i] >= 48 && exp[i] < 58) || isMathOperator(exp[i])))
				return FALSE;
		return TRUE;
	}
	int countOperators(char* exp, int size)
	{
		int count = 0;
		for (int i = 0; i < size; i++)
			if (isMathOperator(exp[i]))
				count++;
		return count;
	}
	int getNextOperIndex(char* opers, int size)
	{
		for (int i = 0; i < size; i++)
			if (opers[i] == '*' || opers[i] == '/')
				return i;
		return 0;
	}
	void perform(int* a, int* b, char oper)
	{
		if (oper == '+')
			*a += *b;
		else if (oper == '-')
			*a -= *b;
		else if (oper == '*')
			*a *= *b;
		else if (oper == '/')
			*a /= *b;
		
	}
	int seperateArrays(char* exp, int size, char* opers, int* nums)
	{
		int i = 0, count = 0, j = 0;
		for (i = 0; i < size; i++)
		{
			if (isMathOperator(exp[i]))
			{
				opers[count] = exp[i];
				exp[i] = 0;
				count++;
			}
		}
		count = 0;
		for (i = 0; i < size; i++)
		{
			if (exp[i] == 0)
				return FALSE;
			nums[count] = atoi(exp + i);
			count++;
			i += strlen(exp + i);
		}
		return TRUE;
	}
	int calculate(char* opers, int* nums, int opersNum)
	{
		if (opersNum == 0)		// end cond
			return nums[0];
		int numsNum = opersNum + 1;
		int iOper = getNextOperIndex(opers, opersNum);		// get next operator
		perform(nums + iOper, nums + iOper + 1, opers[iOper]);		// perform action
		memcpy(nums + iOper + 1, nums + iOper + 2, numsNum - iOper - 2);	// reduce numbers
		memcpy(opers + iOper, opers + iOper + 1, opersNum - iOper - 1);	// reduce operators
		return calculate(opers, nums, opersNum - 1);
	}

	// checks
	const char* exp = getArg(argv, argc, 1);		// get math expression
	int size = strlen(exp);					// get its size

	char* cExp = (char*)umalloc(size + 1);		// copy exp
	memcpy(cExp, exp, size + 1);

	if (argc != 2 || !isValidExp(cExp, size) || size == 0)
	{
		uputs("Invalid syntax. Try \'help bc\'.");
		return;
	}

	// main function
	int opersNum = countOperators(cExp, size);		// get number of operators
	char* opers = (char*)umalloc(opersNum);			// seperate into 2 arrays
	int* nums = (int*)umalloc((opersNum + 1) * sizeof(int));
	if (!seperateArrays(cExp, size, opers, nums))
	{
		uputs("Invalid syntax. Try \'help bc\'.");
		ufree(opers);
		ufree(nums);
		ufree(cExp);
		return;
	}
	int res = calculate(opers, nums, opersNum);		// get exp result
	uputi(res);		// print result

	ufree(opers);
	ufree(nums);
	ufree(cExp);
}

void ls(char* argv, int argc)
{
	char path[200] = {0};
	memcpy(path, dir, strlen(dir));
	if (argc == 2)
	{
		const char* rel = getArg(argv, argc, 1);
		if (rel[0] == '/')
			memcpy(path, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(path + strlen_dir, rel, strlen(rel) + 1);
		}
	}
	else if (argc > 2)
	{
		uputs("Invalid syntax. Try \'help ls\'.");
		return;
	}
	int size = ufile_size(path);

	if (size > 0)	
	{
		char* data = (char*)umalloc(size + 1);
		uread_file(path, data);
		data[size] = 0;
		int lines = strsplit(data, '\n');
		for (int i = 0; i < lines; i++)
		{
			const char* line = getArg(data, lines, i);
			if (strlen(line) == 0)
				continue;
			const int len = strfind(line, ',');
			for (int j = 0; j < len; j++)
				uputc(line[j]);
			uputc('\n');
		}
		ufree(data);
	}
}

void pwd(char* argv, int argc)
{
	uputs(dir);
}

void cd(char* argv, int argc)
{
	if (argc == 2)
	{
		char temp[200] = {0};
		memcpy(temp, dir, 200);

		const char* rel = getArg(argv, argc, 1);
		if (rel[0] == '/')
			memcpy(dir, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(dir + strlen_dir, rel, strlen(rel) + 1);
		}
		if (dir[strlen(dir) - 1] != '/')
		{
			dir[strlen(dir) + 1] = 0;
			dir[strlen(dir)] = '/';
		}

		if (ufile_type(dir) != 1)
		{
			memcpy(dir, temp, 200);
			uputs("Not a folder.");
		}
	}
	else
		uputs("Invalid syntax. Try \'help cd\'.");
}

void cat(char* argv, int argc)
{
	char path[200] = {0};
	memcpy(path, dir, strlen(dir));
	if (argc == 2)
	{
		const char* rel = getArg(argv, argc, 1);
		if (rel[0] == '/')
			memcpy(path, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(path + strlen_dir, rel, strlen(rel) + 1);
		}

		int size = ufile_size(path);
		if (size > 0)
		{
			char* data = (char*)umalloc(size);
			uread_file(path, data);
			for (int i = 0; i < size; i++)
				uputc(data[i]);
			ufree(data);
		}
	}
	else
		uputs("Invalid syntax. Try \'help cat\'.");
}

void rm(char* argv, int argc)
{
	char path[200] = {0};
	memcpy(path, dir, strlen(dir));
	if (argc == 2)
	{
		const char* rel = getArg(argv, argc, 1);
		if (rel[0] == '/')
			memcpy(path, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(path + strlen_dir, rel, strlen(rel) + 1);
		}

		udelete_file(path);
	}
	else
		uputs("Invalid syntax. Try \'help rm\'.");
}

void touch(char* argv, int argc)
{
	char path[200] = {0};
	memcpy(path, dir, strlen(dir));
	if (argc == 2)
	{
		const char* rel = getArg(argv, argc, 1);
		if (rel[0] == '/')
			memcpy(path, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(path + strlen_dir, rel, strlen(rel) + 1);
		}

		ucreate_file(path);
	}
	else
		uputs("Invalid syntax. Try \'help touch\'.");
}

void mkdir(char* argv, int argc)
{
	char path[200] = {0};
	memcpy(path, dir, strlen(dir));
	if (argc == 2)
	{
		const char* rel = getArg(argv, argc, 1);
		if (rel[0] == '/')
			memcpy(path, rel, strlen(rel) + 1);
		else{
			int strlen_dir = strlen(dir);
			memcpy(path + strlen_dir, rel, strlen(rel) + 1);
		}

		ucreate_folder(path);
	}
	else
		uputs("Invalid syntax. Try \'help mkdir\'.");
}