#include "string.h"

int strlen(char* str)
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

int strncmp(char* str1, char* str2, int n)
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