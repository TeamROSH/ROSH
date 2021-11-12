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

	int size = 4;
	int* pNum = (int*)kmalloc(sizeof(int));
	for (int i = 0; i < size; i++)
	{
		pNum[i] = 200 + i * 10;
	}
	for (int i = 0; i < size; i++)
	{
		puti(pNum[i]);
		putc('\n');
	}
	kfree(pNum);

	printLogo();		// print ROSH
}

void printLogo()
{
	char logo[] = "Kernel Loaded.\nGDT initialized.\nIDT initialized.\n\n"
				"\t\t\t\t\t\t\t______ _____ _____ _   _ \n"
				"\t\t\t\t\t\t\t| ___ \\  _  /  ___| | | |\n"
				"\t\t\t\t\t\t\t| |_/ / | | \\ `--.| |_| |\n"
				"\t\t\t\t\t\t\t|    /| | | |`--. \\  _  |\n"
				"\t\t\t\t\t\t\t| |\\ \\\\ \\_/ /\\__/ / | | |\n"
				"\t\t\t\t\t\t\t\\_| \\_|\\___/\\____/\\_| |_/\n"
				"\t\t\t\t  Roei Aviad and Yoav Shaham's Operating System!\n\n";
	puts(logo);
}