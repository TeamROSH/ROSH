#include "../../libc/screen.h"
#include "../../libc/system.h"
#include <stdint.h>
#include "../GDT/gdt.h"
#include "../IDT/idt.h"
#include "../memory/paging.h"
#include "../memory/heap.h"
#include "../process/process.h"
#include "../../fs/fs.h"
#include "../networking/drivers/ethernet_driver.h"
#include "../networking/protocols/ethernet.h"

/*
	print ROSH logo
*/
void printLogo();
void kernelShutdown();

extern void usermode(void);

void main() {
	gdt_initialize();		// initializing gdt
	
	idt_initialize();		// initializing idt
	initKernelHeap();		// init kernel heap
	initialize_paging();	// init paging
	keyboard_initialize();	// initializing keyboard
	initialize_ethernet_driver();
	
	initConsole();			// init cursor
	init_fs();				// init file system

	uint8_t msg[6] = "hello";
	uint8_t destination_address[6] = {255, 255, 255, 255, 255, 255};
	send_ethernet_packet(msg, 6, 0x806, destination_address);


	printLogo();		// print ROSH

	getchar();
	clearConsole();

	process_init();
}

void printLogo()
{
	char logo[] = "Kernel Loaded.\nGDT initialized.\nIDT initialized.\nHeap initialized.\nPaging initialized.\n\n"
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