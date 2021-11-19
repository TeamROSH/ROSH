#ifndef STRINGH
#define STRINGH

#define ASCII_NUM_OFFSET 48

/*
	get length of str
	@param str: str to check
	@return length of string
*/
int strlen(char* str);

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
int atoi(char* str);

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
int strncmp(char* str1, char* str2, int n);

#endif