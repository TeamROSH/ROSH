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
	check if str2 is in str1
	@param str1: str to check
	@param str2: str to check
	@retrun (char*) - str2 in str1, (NULL) - str2 not in str1
*/
char* strstr(const char* str1, const char* str2);


/*
	breaks a string into a sequence of zero or more nonempty tokens
	@param s: str to check
	@param d: char to check
	@retrun (token) - d in s, (NULL) - last token
*/
char* strtok(char* s, char d);
#endif