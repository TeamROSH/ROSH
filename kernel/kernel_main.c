#include "gdt.h"

void main() {
	//initializing gdt
	gdt_initialize();

	char* video_memory = (char*) 0xb8000;
	*video_memory = 'X';



	while (1) {}
}