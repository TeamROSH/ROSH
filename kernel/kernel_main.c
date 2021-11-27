#include "../libc/screen.h"
#include "../libc/system.h"
#include <stdint.h>
#include "GDT/gdt.h"
#include "IDT/idt.h"
#include "memory/paging.h"
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
	initKernelHeap();		// init heap
	initialize_paging();	// init paging
	//memset(VIDEO_MEM_START, 0, PAGE_SIZE);
	/*
	uint32_t* x = 0x7504001;
	uint32_t* y = 0x7504000;
	for(int i = 0; i < 10; i+=2)
	{
		*y = 'F';
		y+= 2;
		*x = 'X';
		x+= 2;
	}*/
	keyboard_initialize();	// initializing keyboard
	  initConsole();			// init cursor

	printLogo();		// print ROSH
	putc('a');
	 getchar();
	 clearConsole();

	//umain();
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
				"\t\t\t\t  Roei Aviad and Yoav Shaham's Operating System!\n\nPress Enter to start!\n";
	puts(logo);
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