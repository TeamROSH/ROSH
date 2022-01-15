#ifndef STRINGH
#define STRINGH

#define ASCII_NUM_OFFSET 48
#define SHIFT_B64 43
#define MAX_CHAR 255
#include <stdint.h>

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

/*
    This function calculates the base64 output length of a encoded string
    @param encoded_string: the encoded string  
    This function returns the encoded string value
*/
int calc_base64_output(char* encoded_string);

/*
	split string by char. accessed by getArg function
	@param str: the string
	@param devidor: the char devidor
	@returns number of parts created
*/
int strsplit(char* str, char devidor);

/*
	get arguments from splitted string
	@param argv: splitted string
	@param argc: value returned from split
	@param argNum: equivalent of str[argNum]
	@returns string in array index argNum
*/
const char* getArg(const char* argv, int argc, int argNum);

/*
	get index of char in string
	@param str: the string
	@param c: the char
	@returns index of the char
*/
int strfind(const char* str, char c);

#endif