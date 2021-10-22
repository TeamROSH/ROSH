#include "../libc/screen.h"
#include <stdint.h>

/*
	print ROSH logo
*/
void printLogo();

void main() {
	printLogo();
	char numstr[] = "This is a number:\t";
	int num = 2265;
	puts(numstr);
	puti(num);
	putc('\n');
	puts("End of Line");
	puts("\rLOL");

	while (1) {}
}

void printLogo()
{
	char logo[] = "\t\t\t\t\t\t\t______ _____ _____ _   _ \n"
				"\t\t\t\t\t\t\t| ___ \\  _  /  ___| | | |\n"
				"\t\t\t\t\t\t\t| |_/ / | | \\ `--.| |_| |\n"
				"\t\t\t\t\t\t\t|    /| | | |`--. \\  _  |\n"
				"\t\t\t\t\t\t\t| |\\ \\\\ \\_/ /\\__/ / | | |\n"
				"\t\t\t\t\t\t\t\\_| \\_|\\___/\\____/\\_| |_/\n\n\n";
	puts(logo);
}