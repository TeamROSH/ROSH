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
	return i;
}