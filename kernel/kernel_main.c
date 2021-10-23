#include "gdt.h"

void main() {


	char* video_memory = (char*) 0xb8000;
	*video_memory = 'X';

	//initializing gdt
	gdt_initialize();

	while (1) {}
}