#ifndef IDT_H
#define IDT_H
#include <stdint.h>

typedef struct {
   uint16_t base_low; // base bits 0-15
   uint16_t selector;  //16 or 32 bit mod
   uint8_t zero;      // unused,
   uint8_t flags;      // intterupt flags
   uint16_t base_high; // base bits 16-31
}__attribute__((packed)) idt_gate;


typedef struct{
    uint16_t limit; //idt limit
    uint32_t base;  //first idt gate
}__attribute__((packed)) idt_pointer;

/*
    This function initaize the idt
*/
void idt_initialize();


#endif