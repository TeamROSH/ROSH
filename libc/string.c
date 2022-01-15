#include "string.h"
//extern Heap g_kernelHeap;

int8_t g_decode_table[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51 };


int base64_decode(char* encoded_string, char* decoded_string);
int calc_base64_output(char* encoded_string);

int strlen(const char* str)
{
    int i = 0;
    while (*str != 0)    // while not end
    {
        i++;    // continue
        str++;
    }
    return i;        // return length on end
}

void itoa(int num, char* res)
{
    int size = digits(num);
    for (int i = 0; i < size; i++)        // for size times
    {
        res[size - i - 1] = num % 10 + ASCII_NUM_OFFSET;        // add last digit
        num /= 10;        // remove last digit
    }
}

int digits(int num)
{
    int i = 0;
    while (num > 0)        // count num's digits
    {
        num /= 10;
        i++;
    }
    return i == 0 ? 1 : i;
}

int strncmp(const char* str1, const char* str2, int n)
{
    for (int i = 0; i < n; i++)        // n times (or one of strings over)
    {
        if (str1[i] == 0 || str2 == 0)
            return str1[i] == str2[i] ? 0 : str1[i] - str2[i];
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
    }
    return 0;
}

int atoi(const char* str)
{
    int size = strlen(str);
    int num = 0;
    for (int i = 0; i < size; i++)        // for every const char*
    {
        if (str[i] >= ASCII_NUM_OFFSET && str[i] < ASCII_NUM_OFFSET + 10)        // if in range (0-9)
        {
            num *= 10;            // add to int
            num += str[i] - ASCII_NUM_OFFSET;
        }
    }
    return num;
}

int base64_decode(char* encoded_string, char* decoded_string)
{    
    int encoded_length = 0;
    uint32_t curr_dword = 0;

    // if empry input string or invalid string len
    if( encoded_string == NULL)
    {
        return 0;
    }

    // calculating string encoded string length
    encoded_length = strlen(encoded_string);

    //if invalid string length
    if(encoded_length % 4 != NULL)
    {
        return 0;
    }


    for(int i = 0, j = 0; i < encoded_length; i+= 4, j+=3)
    {
        // going through decoded string and converting to ascii
        curr_dword = g_decode_table[encoded_string[i] - SHIFT_B64];
        curr_dword = (curr_dword << 6) | g_decode_table[encoded_string[i + 1] - SHIFT_B64];
        curr_dword = encoded_string[i + 2] == '=' ? curr_dword << 6: (curr_dword << 6) | g_decode_table[encoded_string[i + 2] - SHIFT_B64];
        curr_dword = encoded_string[i + 3] == '=' ? curr_dword << 6 : (curr_dword << 6) | g_decode_table[encoded_string[i + 3] - SHIFT_B64];

        // cping the encoded daa into the output
        decoded_string[j] = (curr_dword >> 16) & MAX_CHAR;
        if (encoded_string[i+2] != '=')
            decoded_string[j+1] = (curr_dword >> 8) & MAX_CHAR;
        if (encoded_string[i+3] != '=')
            decoded_string[j+2] = curr_dword & MAX_CHAR;
        
    }

    return 1;
}

int calc_base64_output(char* encoded_string)
{
    return strlen(encoded_string) / 4 * 3;
}
