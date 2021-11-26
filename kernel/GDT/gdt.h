#ifndef GDT_H
#define GDT_H
#include <stdint.h>

#define GDT_ENTRIES 6
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
uint32_t base;  //first gdt entry
}__attribute__((packed)) gdt_pointer;

/*
	GDT entry detailed struct
	Same size as gdt_entry
*/
typedef struct gdt_entry_bits {
	unsigned int limit_low              : 16;
	unsigned int base_low               : 24;
	unsigned int accessed               :  1;
	unsigned int read_write             :  1;
	unsigned int conforming				:  1;
	unsigned int code                   :  1; // 1 for code, 0 for data
	unsigned int code_data_segment      :  1; // (is not TSS or LDT?)
	unsigned int DPL                    :  2; // privilege level
	unsigned int present                :  1;
	unsigned int limit_high             :  4;
	unsigned int available              :  1;
	unsigned int long_mode              :  1;
	unsigned int big                    :  1;
	unsigned int gran                   :  1; // 1 for 4k page addressing, 0 for byte addressing
	unsigned int base_high              :  8;
} __attribute__((packed)) gdt_entry_bits;

/*
    This function initaize the gdt
*/
void gdt_initialize();



#endif