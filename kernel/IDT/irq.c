#include "irq.h"

interrupt_handler g_interrupt_handlers[IDT_ENTRIES];

void pic_initialize()
{
    //remapping irq to idt 0x20-0x2f
    outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_handler(registers_t* registers)
{   
    //sending ack to pic master
    outb(0x20, 0x20);

    if(registers->interrupt_num > IRQ8)
    {
        //sending ack to pic slave
        outb(0xA0, 0x20);
    }
    
    //if there is a handler for the irq
    if(g_interrupt_handlers[registers->interrupt_num] != 0)
    {
        g_interrupt_handlers[registers->interrupt_num](registers);
    }
    //if no handler for irq
    else
    {
        puts("invalid irq called");
    }

}