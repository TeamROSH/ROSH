#ifndef GDT_H
#define GDT_H
#include "types.h"

#define GDT_ENTRIES 5
typedef struct 
{
    uint16_t limit; // 20 bit maximum addressable unit
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access; //access to segment
    uint8_t granularity;    // contains limit and flags
    uint8_t base_high; // 32 base contains base linear address of segment

}__attribute__((packed)) gdt_entry;

//points to gdt table
typedef struct{
uint16_t limit;   //table limit 
uint base;  //first gdt entry
}__attribute__((packed)) gdt_pointer;

/*
    This function initaize the gdt
    Input - none
    Output - none 
*/
void gdt_initialize();



#endif