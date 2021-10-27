#include "isr.h"

extern interrupt_handler g_interrupt_handlers[IDT_ENTRIES];

void isr_handler(registers_t registers)
{
    //if there is a handler for the isr
    if(g_interrupt_handlers[registers.interrupt_num] != 0)
    {
        g_interrupt_handlers[registers.interrupt_num](registers);
    }
    //if no handler for isr
    else
    {
        puts("invalid isr called");
    }
}