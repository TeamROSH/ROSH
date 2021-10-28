#ifndef ISR_H
#define ISR_H
#include "irq.h"
/*
    This function handles idt interrupt
    @registers: registers values 
*/
void isr_handler(registers_t registers);


/*
    This function sets interrpt handler in the interrupt handlers array
    @interrupt_num: index into the interrupt handler array
    @interrupt: the interrupt handler function 
*/
void set_interrupt(uint8_t interrupt_num, interrupt_handler interrupt);

#endif