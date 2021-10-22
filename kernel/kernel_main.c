#include "../libc/screen.h"
#include <stdint.h>

void main() {
	char welcome[] = "Welcome to ROSH! This is a number: ";
	puts(welcome);
	int num = 2265;
	puti(num);

	while (1) {}
}