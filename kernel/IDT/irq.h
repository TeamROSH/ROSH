#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "../../libc/screen.h"
#include "../ports.h"

#define IDT_ENTRIES 256

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


typedef struct {
    uint32_t es;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t interrupt_num, err_code;             
    uint32_t eip, cs, eflags, user_esp, ss;           //cpu pushes

}registers_t;

typedef void (*interrupt_handler)(registers_t* registers);

/*
    initializes the pic 
*/
void pic_initialize();

/*
    This function handles irq according to the irq type
    @param registers: registers values 
*/
void irq_handler(registers_t* registers);

#endif