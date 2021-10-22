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
	puts("End of Line");
	puts("\rLOL");

	while (1) {}
}