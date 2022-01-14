#ifndef STRINGH
#define STRINGH

#define ASCII_NUM_OFFSET 48
#define SHIFT_B64 43
#define MAX_CHAR 255
#include <stdint.h>
#include "../kernel/memory/heap.h"

#ifndef NULL
#define NULL 0
#endif

/*
	get length of str
	@param str: str to check
	@return length of string
*/
int strlen(const char* str);

/*
	int to array
	@param num: the int to convert
	@param res: pointer to string
*/
void itoa(int num, char* res);

/*
	array to int
	@param str: pointer to string
	@returns number
*/
int atoi(const char* str);

/*
	get number of digits from int
	@param num: number to check
	@return number of digits
*/
int digits(int num);

/*
	compare 2 strings
	@param str1: str to check
	@param str1: str to check
	@param n: number of chars to check
	@return (<0) - str1<str2, (>0) - str1>str2, (=0) - str1=str2
*/
int strncmp(const char* str1, const char* str2, int n);


/*
	This function decodes base64 encoded string 
	@param encoded_string: the base 64 encoded string
	@parma decoded_string: the decoded string 
	the function returns 1 if successful and 0
	 if not 
*/
int base64_decode(char* encoded_string, char* decoded_string);
#endif