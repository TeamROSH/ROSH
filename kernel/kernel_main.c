#include "../libc/screen.h"
#include <stdint.h>
#include "GDT/gdt.h"
#include "IDT/idt.h"

/*
	print ROSH logo
*/
void printLogo();

void main() {
	// initializing gdt
	gdt_initialize();
	idt_initialize();
	initConsole();		// init cursor
	printLogo();		// print ROSH
	int x =1/0;
	while (1) {}
}

void printLogo()
{
	char logo[] = "GDT initialized.\nKernel Loaded.\n\n"
				"\t\t\t\t\t\t\t______ _____ _____ _   _ \n"
				"\t\t\t\t\t\t\t| ___ \\  _  /  ___| | | |\n"
				"\t\t\t\t\t\t\t| |_/ / | | \\ `--.| |_| |\n"
				"\t\t\t\t\t\t\t|    /| | | |`--. \\  _  |\n"
				"\t\t\t\t\t\t\t| |\\ \\\\ \\_/ /\\__/ / | | |\n"
				"\t\t\t\t\t\t\t\\_| \\_|\\___/\\____/\\_| |_/\n"
				"\t\t\t\t  Roei Aviad and Yoav Shaham's Operating System!\n\n";
	puts(logo);
}