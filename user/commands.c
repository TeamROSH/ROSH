#include "commands.h"
#include "../libc/screen.h"
#include "../kernel/IDT/keyboard.h"
#include "../kernel/memory/heap.h"
#include "../libc/string.h"
#include "../libc/system.h"
#include "../libc/memory.h"
#define NULL 0
#define FALSE 0
#define TRUE !FALSE

const char* getArg(const char* argv, int argc, int argNum)
{
	if (argNum >= argc)		// prevent buffer overflow
		return NULL;
	const char* res = argv;
	for (int i = 0; i < argNum; i++)		// run until wanted argument reached
	{
		res += strlen(res) + 1;		// next argument
	}
	return res;
}

void grep(char* argv, int argc)
{
	char *token = NULL;

	// validating length
	if(argc != 3)
	{
		puts("Invalid syntax. Try \'help grep\'.");
	}

	char *str = getArg(argv, argc, 2);
	char *substr = getArg(argv, argc, 1);
	else
	{

		token = strtok(str, "\n");
		
		// going through the lines in the string
		while(token != NULL)
		{
			// if string included
			if(strstr(token, substr) != 0)
			{
				puts(token)
			}
			token = strtok(NULL, str);
		}
	}
}

void unknown_command(char* argv, int argc)
{
	puts("Command \'");
	puts(argv);
	puts("\' not found. Try \'help\'.");
}

void echo(char* argv, int argc)
{
	for (int i = 1; i < argc; i++)		// for every argument except command name
	{
		puts(getArg(argv, argc, i));		// print it
		putc(' ');		// add the space
	}
}

void color(char* argv, int argc)
{
	if (argc != 3)
		puts("Invalid syntax. Try \'help color\'.");
	else
	{
		char fore = (char)atoi(getArg(argv, argc, 1));		// get foreground color
		char back = (char)atoi(getArg(argv, argc, 2));		// get background color
		setScreenColor(fore, back);				// set screen colors
	}
}

void help(char* argv, int argc)
{
	if (argc == 1)		// no specific command
	{
		puts(
			"For specific command:\n"
			"help <command name>\n\n"
			"Available Commands:\n"
			"help - Print information about available commands.\n"
			"echo - Print message to the screen.\n"
			"color - Change the shell's colors.\n"
			"grep - \n"
			"shutdown - shutdown the computer.\n"
			"bc - basic calculator.\n"
		);
	}
	else if (argc == 2)		// for specific command
	{
		for (int i = 0; i < FUNS_NUM; i++)		// search functions
		{
			if (strncmp(getArg(argv, argc, 1), fun_names[i], strlen(fun_names[i])) == 0)		// if found
			{
				puts(getArg(fun_info, FUNS_NUM, i));		// print info
				return;
			}
		}
		puts("Unknown command. Try \'help\'.");
	}
	else
	{
		puts("Invalid syntax. Try \'help help\'.");
	}
}

void shutdownCommand(char* argv, int argc)
{
	puts("Are you sure? (y/n): ");
	bflush();
	char res = getchar();
	if (res == 'y')
		shutdown();
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

	char* cExp = (char*)kmalloc(size + 1);		// copy exp
	memcpy(cExp, exp, size + 1);

	if (argc != 2 || !isValidExp(cExp, size) || size == 0)
	{
		puts("Invalid syntax. Try \'help bc\'.");
		return;
	}

	// main function
	int opersNum = countOperators(cExp, size);		// get number of operators
	char* opers = (char*)kmalloc(opersNum);			// seperate into 2 arrays
	int* nums = (int*)kmalloc((opersNum + 1) * sizeof(int));
	if (!seperateArrays(cExp, size, opers, nums))
	{
		puts("Invalid syntax. Try \'help bc\'.");
		kfree(opers);
		kfree(nums);
		kfree(cExp);
		return;
	}
	int res = calculate(opers, nums, opersNum);		// get exp result
	puti(res);		// print result

	kfree(opers);
	kfree(nums);
	kfree(cExp);
}