#include "paging.h"

uint8_t g_pages_array[PAGES];

void allow_paging();


void allow_paging()
{
	asm("movl %cr0, %eax");
    //enabling paging
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}