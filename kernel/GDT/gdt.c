#include "gdt.h"

gdt_entry g_gdt_entries[GDT_ENTRIES];
gdt_pointer g_gdt_pointer;

extern void load_gdt(uint32_t);


void gdt_initialize()
{

    //initialize gdt pointer
    g_gdt_pointer.limit = sizeof(gdt_entry) * GDT_ENTRIES - 1;
    g_gdt_pointer.base = (uint32_t)&g_gdt_entries[0];

    //first segment is empty (0x00)
    g_gdt_entries[0].limit = 0x0;
    g_gdt_entries[0].base_low = 0x0;
    g_gdt_entries[0].base_middle = 0x0;
    g_gdt_entries[0].access = 0x0;
    g_gdt_entries[0].granularity = 0x0;
    g_gdt_entries[0].base_high = 0x0; 
    
    //kernel code segment (0x08)
    g_gdt_entries[1].limit = 0xFFFF;
    g_gdt_entries[1].base_low = 0x0;
    g_gdt_entries[1].base_middle = 0x0;
    g_gdt_entries[1].access = 0x9A;
    g_gdt_entries[1].granularity = 0xCF;
    g_gdt_entries[1].base_high = 0x0;

    //kernel data segment (0x10)
    g_gdt_entries[2].limit = 0xFFFF;
    g_gdt_entries[2].base_low = 0x0;
    g_gdt_entries[2].base_middle = 0x0;
    g_gdt_entries[2].access = 0x92;
    g_gdt_entries[2].granularity = 0xCF;
    g_gdt_entries[2].base_high = 0x0;

    /*
		user code and data segments:
		code (0x18), data (0x20)
	*/
	gdt_entry_bits* user_code = (gdt_entry_bits*)(g_gdt_entries + 3);
	gdt_entry_bits* user_data = (gdt_entry_bits*)(g_gdt_entries + 4);

	// init user code
	user_code->limit_low = 0xFFFF;
	user_code->base_low = 0;
	user_code->accessed = 0;
	user_code->read_write = 1; // readable
	user_code->conforming = 0;
	user_code->code = 1;		// code not data
	user_code->code_data_segment = 1;
	user_code->DPL = 3; // ring 3
	user_code->present = 1;
	user_code->limit_high = 0xF;
	user_code->available = 1;
	user_code->long_mode = 0;
	user_code->big = 1; // 32 bits
	user_code->gran = 1; // 4KB page addressing
	user_code->base_high = 0;

	// init user data
	*user_data = *user_code;		// same except data and not code
	user_data->code = 0;

    //loading the new gdt into memory
    load_gdt((uint32_t)&g_gdt_pointer);
}

