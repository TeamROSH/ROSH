#include "../libc/screen.h"
#include "../libc/system.h"
#include <stdint.h>
#include "GDT/gdt.h"
#include "IDT/idt.h"
#include "memory/heap.h"
#include "../user/user_main.h"

/*
	print ROSH logo
*/
void printLogo();
void kernelShutdown();

void main() {
	gdt_initialize();		// initializing gdt
	idt_initialize();		// initializing idt
	keyboard_initialize();	// initializing keyboard
	initConsole();			// init cursor
	initKernelHeap();		// init heap

	printLogo();		// print ROSH
	umain();
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
				"\t\t\t\t  Roei Aviad and Yoav Shaham's Operating System!\n\nPress any key to start!\n";
	puts(logo);
	clearOnPrint();			// !!! no prints after this line !!!
}

/*
	clean shutdown
*/
void kernelShutdown()
{
	kfree(getTrackerUp());
	kfree(getTrackerDown());

	shutdown();
}