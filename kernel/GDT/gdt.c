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

    //user code segment (0x18)
    g_gdt_entries[3].limit = 0xFFFF;
    g_gdt_entries[3].base_low = 0x0;
    g_gdt_entries[3].base_middle = 0x0;
    g_gdt_entries[3].access = 0xFA;
    g_gdt_entries[3].granularity = 0xCF;
    g_gdt_entries[3].base_high = 0x0;     

    //user data segment (0x20)
    g_gdt_entries[4].limit = 0xFFFF;
    g_gdt_entries[4].base_low = 0x0;
    g_gdt_entries[4].base_middle = 0x0;
    g_gdt_entries[4].access = 0xF2;
    g_gdt_entries[4].granularity = 0xCF;
    g_gdt_entries[4].base_high = 0x0;  

    

    //loading the new gdt into memory
    load_gdt((uint32_t)&g_gdt_pointer);
}

