#include "../libc/screen.h"
#include <stdint.h>

void main() {
	char welcome[] = "Welcome to ROSH!\n";
	char numstr[] = "This is a number:\t";
	int num = 2265;
	puts(welcome);
	puts(numstr);
	puti(num);
	putc('\n');
	for (int i = 0; i < 30; i++)
		puts(welcome);

	while (1) {}
}