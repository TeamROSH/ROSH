#ifndef STRINGH
#define STRINGH

/*
	get length of str
	Input: char array
	Output: str length
*/
int strlen(char* str);

/*
	int to array
	Input: int, char pointer to string result, number of chars to put
*/
void itoa(int num, char* res, int size);

#endif