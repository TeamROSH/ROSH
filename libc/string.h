#ifndef STRINGH
#define STRINGH

#define ASCII_NUM_OFFSET 48

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
	copies value c into str n times
	@param str: string that will be copied into 
	@pram c: int value that will be copied
	@param n: size_t number of compies
*/
void memset(void *str, int c, size_t n);
#endif