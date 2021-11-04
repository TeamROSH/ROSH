#ifndef IDT_H
#define IDT_H
#include <stdint.h>
#include "isr.h"

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

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void isr128();

#endif
