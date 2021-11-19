#include "user_main.h"
#include "../kernel/IDT/keyboard.h"
#include "../libc/screen.h"
#define INPUT_SIZE 21

void umain()
{
	char input[INPUT_SIZE] = {0};
	while (1)
	{
		puts("\n$ ");
		getline(input, INPUT_SIZE - 1);
		// puts(cmd(input))
		puts("Got Command!\n");
	}
}