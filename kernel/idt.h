#ifndef IDT_H
#define IDT_H
#include <stdint.h>

#define IDT_ENTRIES 256

typedef struct {
   uint16_t base_low; // base bits 0-15
   uint16_t selector;  // load kernel code segment from the gdt according to selector
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

/*
    This fuction initializes specific idt gate
    @param gate_num: igt_gate index
    @param base: isr (or irq) function base address

*/
void idt_gate_initialize(uint32_t gate_num,
 uint32_t base, 
 uint16_t selector, 
 uint8_t flags);


#endif