#include "../libc/screen.h"
#include <stdint.h>
#include "GDT/gdt.h"
#include "IDT/idt.h"
#include "Memory/heap.h"

/*
	print ROSH logo
*/
void printLogo();

void main() {
	gdt_initialize();		// initializing gdt
	idt_initialize();		// initializing idt
	keyboard_initialize();	// initializing keyboard
	initConsole();			// init cursor
	initKernelHeap();		// init heap

	printLogo();		// print ROSH
}

void printLogo()
{
	char logo[] = "Kernel Loaded.\nGDT initialized.\nIDT initialized.\nHeap initialized.\n\n"
				"\t\t\t\t\t\t\t______ _____ _____ _   _ \n"
				"\t\t\t\t\t\t\t| ___ \\  _  /  ___| | | |\n"
				"\t\t\t\t\t\t\t| |_/ / | | \\ `--.| |_| |\n"
				"\t\t\t\t\t\t\t|    /| | | |`--. \\  _  |\n"
				"\t\t\t\t\t\t\t| |\\ \\\\ \\_/ /\\__/ / | | |\n"
				"\t\t\t\t\t\t\t\\_| \\_|\\___/\\____/\\_| |_/\n"
				"\t\t\t\t  Roei Aviad and Yoav Shaham's Operating System!\n\n";
	puts(logo);
}