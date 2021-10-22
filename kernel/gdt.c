#include "gdt.h"

GDTEntry gdtEntries[GDT_ENTRIES];
GDTPointer gdtPointer;

extern void load_gdt(uint);


void gdt_initialize()
{

    //initialize gdt pointer
    gdtPointer.limit = sizeof(GDTEntry) * GDT_ENTRIES - 1;
    gdtPointer.base = (uint)&gdtEntries[0];

    //first segment is empty
    gdtEntries[0].limit = 0x0;
    gdtEntries[0].baseLow = 0x0;
    gdtEntries[0].baseMiddle = 0x0;
    gdtEntries[0].access = 0x0;
    gdtEntries[0].granularity = 0x0;
    gdtEntries[0].baseHigh = 0x0; 
    
    //kernel code segment 
    gdtEntries[1].limit = 0xFFFF;
    gdtEntries[1].baseLow = 0x0;
    gdtEntries[1].baseMiddle = 0x0;
    gdtEntries[1].access = 0x9A;
    gdtEntries[1].granularity = 0xCF;
    gdtEntries[1].baseHigh = 0x0;

    //kernel data segment 
    gdtEntries[2].limit = 0xFFFF;
    gdtEntries[2].baseLow = 0x0;
    gdtEntries[2].baseMiddle = 0x0;
    gdtEntries[2].access = 0x92;
    gdtEntries[2].granularity = 0xCF;
    gdtEntries[2].baseHigh = 0x0;

    //user code segment 
    gdtEntries[3].limit = 0xFFFF;
    gdtEntries[3].baseLow = 0x0;
    gdtEntries[3].baseMiddle = 0x0;
    gdtEntries[3].access = 0xFA;
    gdtEntries[3].granularity = 0xCF;
    gdtEntries[3].baseHigh = 0x0;     

    //user data segment 
    gdtEntries[4].limit = 0xFFFF;
    gdtEntries[4].baseLow = 0x0;
    gdtEntries[4].baseMiddle = 0x0;
    gdtEntries[4].access = 0xF2;
    gdtEntries[4].granularity = 0xCF;
    gdtEntries[4].baseHigh = 0x0;  

    

    //loading the new gdt into memory
    load_gdt((uint)&gdtPointer);
}

