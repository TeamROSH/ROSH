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
	get number of digits from int
	@param num: number to check
	@return number of digits
*/
int digits(int num);

#endif