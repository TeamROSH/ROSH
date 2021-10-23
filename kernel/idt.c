#include "idt.h"

idt_gate g_idt_gates[IDT_ENTRIES];
idt_pointer g_idt_pointer;

void idt_initialize();
void idt_gate_initialize(uint32_t gate_num, uint32_t base, uint16_t selector, uint8_t flags);


void idt_initialize()
{

}

void idt_gate_initialize(uint32_t gate_num,
 uint32_t base, 
 uint16_t selector, 
 uint8_t flags)
 {
     g_idt_gates[gate_num].base_low = gate_num & 0xFFFF;    //getting low 16 bits
     g_idt_gates[gate_num].base_high = (gate_num >> 16) & 0xFFFF;   //getting high 16 bits
     g_idt_gates[gate_num].zero = 0;
     g_idt_gates[gate_num].selector = selector;
     g_idt_gates[gate_num].flags = flags;
 }