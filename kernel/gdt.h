#ifndef GDT_H
#define GDT_H
#include "types.h"

typedef struct 
{
    uint16_t limit; // 20 bit maximum addressable unit
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access; //access to segment
    uint8_t granularity;    // contains limit and flags
    uint8_t baseHigh; // 32 base contains base linear address of segment

}__attribute__((packed)) GDTEntry;

//points to gdt table
typedef struct{
uint16_t limit;   //table limit 
uint base;  //first gdt entry
}__attribute__((packed)) GDTPointer;

#endif