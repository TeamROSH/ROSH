#include "string.h"
#include "kernel/memory/heap.h"

char* strtok(char* s, char d)
{
	int i = 0;
	static char* curr_index = NULL;

	// if the string or the delmim null
	if(!s || !d )
	{
		return NULL;
	}

	// last token
	if(curr_index == NULL)
	{
		return NULL;
	}

	char *str = (char* )heap_malloc(strlen(curr_index) + 1);

	for(i = 0; curr_index[i] != '\0'; i++)
	{

		if(curr_index != d)
		{
			str[i] = curr_index[i];
		}

		else
		{
			str[i] = '\0';
			curr_index+= i;
			curr_index+= 1;
			return str;
		}
	}

	str[i] = '\0';
	curr_index = NULL;

	return str;
}

char* strstr(const char* str1, const char* str2)
{
	// while not string end
	while(*str1 != NULL)
	{
		if((*str1 == *str2) && !strncmp(str1, str2, strlen(str1)))
		{
			return str1;
		}
		str1++;
	}

	retrun NULL;
}

int strlen(const char* str)
{
	int i = 0;
	while (*str != 0)	// while not end
	{
		i++;	// continue
		str++;
	}
	return i;		// return length on end
}

void itoa(int num, char* res)
{
	int size = digits(num);
	for (int i = 0; i < size; i++)		// for size times
	{
		res[size - i - 1] = num % 10 + ASCII_NUM_OFFSET;		// add last digit
		num /= 10;		// remove last digit
	}
}

int digits(int num)
{
	int i = 0;
	while (num > 0)		// count num's digits
	{
		num /= 10;
		i++;
	}
	return i == 0 ? 1 : i;
}

int strncmp(const char* str1, const char* str2, int n)
{
	for (int i = 0; i < n; i++)		// n times (or one of strings over)
	{
		if (str1[i] == 0 || str2 == 0)
			return str1[i] == str2[i] ? 0 : str1[i] - str2[i];
		if (str1[i] != str2[i])
			return str1[i] - str2[i];
	}
	return 0;
}

int atoi(const char* str)
{
	int size = strlen(str);
	int num = 0;
	for (int i = 0; i < size; i++)		// for every const char*
	{
		if (str[i] >= ASCII_NUM_OFFSET && str[i] < ASCII_NUM_OFFSET + 10)		// if in range (0-9)
		{
			num *= 10;			// add to int
			num += str[i] - ASCII_NUM_OFFSET;
		}
	}
	return num;
}