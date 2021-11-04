#include "idt.h"
#define KERNEL_CS 0x08

idt_gate g_idt_gates[IDT_ENTRIES];
idt_pointer g_idt_pointer;

extern void load_idt(uint32_t);
void idt_initialize();
void idt_gate_initialize(uint32_t gate_num, uint32_t base, uint16_t selector, uint8_t flags);


void idt_initialize()
{
    //initializing idt pointer
    g_idt_pointer.base = (uint32_t)&g_idt_gates[0];
    g_idt_pointer.limit = sizeof(idt_gate) * IDT_ENTRIES - 1; 

	//initialize pic
	pic_initialize();

    //initializing idt gates
    idt_gate_initialize(0,(uint32_t)isr0,KERNEL_CS,0x8e);
	idt_gate_initialize(1,(uint32_t)isr1,KERNEL_CS,0x8e);
	idt_gate_initialize(2,(uint32_t)isr2,KERNEL_CS,0x8e);
	idt_gate_initialize(3,(uint32_t)isr3,KERNEL_CS,0x8e);
	idt_gate_initialize(4,(uint32_t)isr4,KERNEL_CS,0x8e);
	idt_gate_initialize(5,(uint32_t)isr5,KERNEL_CS,0x8e);
	idt_gate_initialize(6,(uint32_t)isr6,KERNEL_CS,0x8e);
	idt_gate_initialize(7,(uint32_t)isr7,KERNEL_CS,0x8e);
	idt_gate_initialize(8,(uint32_t)isr8,KERNEL_CS,0x8e);
	idt_gate_initialize(9,(uint32_t)isr9,KERNEL_CS,0x8e);
	idt_gate_initialize(10,(uint32_t)isr10,KERNEL_CS,0x8e);
	idt_gate_initialize(11,(uint32_t)isr11,KERNEL_CS,0x8e);
	idt_gate_initialize(12,(uint32_t)isr12,KERNEL_CS,0x8e);
	idt_gate_initialize(13,(uint32_t)isr13,KERNEL_CS,0x8e);
	idt_gate_initialize(14,(uint32_t)isr14,KERNEL_CS,0x8e);
	idt_gate_initialize(15,(uint32_t)isr15,KERNEL_CS,0x8e);
	idt_gate_initialize(16,(uint32_t)isr16,KERNEL_CS,0x8e);
	idt_gate_initialize(17,(uint32_t)isr17,KERNEL_CS,0x8e);
	idt_gate_initialize(18,(uint32_t)isr18,KERNEL_CS,0x8e);
	idt_gate_initialize(19,(uint32_t)isr19,KERNEL_CS,0x8e);
	idt_gate_initialize(20,(uint32_t)isr20,KERNEL_CS,0x8e);
	idt_gate_initialize(21,(uint32_t)isr21,KERNEL_CS,0x8e);
	idt_gate_initialize(22,(uint32_t)isr22,KERNEL_CS,0x8e);
	idt_gate_initialize(23,(uint32_t)isr23,KERNEL_CS,0x8e);
	idt_gate_initialize(24,(uint32_t)isr24,KERNEL_CS,0x8e);
	idt_gate_initialize(25,(uint32_t)isr25,KERNEL_CS,0x8e);
	idt_gate_initialize(26,(uint32_t)isr26,KERNEL_CS,0x8e);
	idt_gate_initialize(27,(uint32_t)isr27,KERNEL_CS,0x8e);
	idt_gate_initialize(28,(uint32_t)isr28,KERNEL_CS,0x8e);
	idt_gate_initialize(29,(uint32_t)isr29,KERNEL_CS,0x8e);
	idt_gate_initialize(30,(uint32_t)isr30,KERNEL_CS,0x8e);
	idt_gate_initialize(31,(uint32_t)isr31,KERNEL_CS,0x8e);
	idt_gate_initialize(32,(uint32_t)irq0,KERNEL_CS,0x8e);
	idt_gate_initialize(33,(uint32_t)irq1,KERNEL_CS,0x8e);
	idt_gate_initialize(34,(uint32_t)irq2,KERNEL_CS,0x8e);
	idt_gate_initialize(35,(uint32_t)irq3,KERNEL_CS,0x8e);
	idt_gate_initialize(36,(uint32_t)irq4,KERNEL_CS,0x8e);
	idt_gate_initialize(37,(uint32_t)irq5,KERNEL_CS,0x8e);
	idt_gate_initialize(38,(uint32_t)irq6,KERNEL_CS,0x8e);
	idt_gate_initialize(39,(uint32_t)irq7,KERNEL_CS,0x8e);
	idt_gate_initialize(40,(uint32_t)irq8,KERNEL_CS,0x8e);
	idt_gate_initialize(41,(uint32_t)irq9,KERNEL_CS,0x8e);
	idt_gate_initialize(42,(uint32_t)irq10,KERNEL_CS,0x8e);
	idt_gate_initialize(43,(uint32_t)irq11,KERNEL_CS,0x8e);
	idt_gate_initialize(44,(uint32_t)irq12,KERNEL_CS,0x8e);
	idt_gate_initialize(45,(uint32_t)irq13,KERNEL_CS,0x8e);
	idt_gate_initialize(46,(uint32_t)irq14,KERNEL_CS,0x8e);
	idt_gate_initialize(47,(uint32_t)irq15,KERNEL_CS,0x8e);
	idt_gate_initialize(128,(uint32_t)isr128,KERNEL_CS,0x8e);

	for (int i = 0; i < IDT_ENTRIES; i++)
	{
		set_interrupt(i, general_handler);
	}

    load_idt((uint32_t)&g_idt_pointer);
}

void idt_gate_initialize(uint32_t gate_num,
 uint32_t base, 
 uint16_t selector, 
 uint8_t flags)
 {
     g_idt_gates[gate_num].base_low = base & 0xFFFF;    //getting low 16 bits
     g_idt_gates[gate_num].base_high = (base >> 16) & 0xFFFF;   //getting high 16 bits
     g_idt_gates[gate_num].zero = 0;
     g_idt_gates[gate_num].selector = selector;
     g_idt_gates[gate_num].flags = flags;
 }
