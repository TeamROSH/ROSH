#ifndef ISR_H
#define ISR_H
#include "irq.h"
/*
    This function handles idt interrupt
    @param registers: registers values 
*/
void isr_handler(registers_t registers);

/*
	print interrupt error
	@param registers: registers values
*/
void general_handler(registers_t registers);

/*
    This function sets interrpt handler in the interrupt handlers array
    @param interrupt_num: index into the interrupt handler array
    @param interrupt: the interrupt handler function 
*/
void set_interrupt(uint8_t interrupt_num, interrupt_handler interrupt);

#endif