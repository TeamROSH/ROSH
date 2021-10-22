#include "../libc/screen.h"
#include <stdint.h>

void main() {
	char welcome[] = "Welcome to ROSH! This is a number: ";
	print(welcome);
	int num = 2265;
	print(num);

	while (1) {}
}