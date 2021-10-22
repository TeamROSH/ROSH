#ifndef STRINGH
#define STRINGH

#define ASCII_NUM_OFFSET 48

/*
	get length of str
	Input: char array
	Output: str length
*/
int strlen(char* str);

/*
	int to array
	Input: int, char pointer to string result
*/
void itoa(int num, char* res);

/*
	get number of digits from int
	Input: number
	Output: number of digits
*/
int digits(int num);

#endif