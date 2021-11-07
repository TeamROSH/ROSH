#ifndef TIME_H
#define TIME_H
#include <stdint.h>

typedef struct {
    uint32_t es;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t interrupt_num, err_code;             
    uint32_t eip, cs, eflags, user_esp, ss;           //cpu pushes

}registers_t;

typedef void (*interrupt_handler)(registers_t* registers);


/*
    This function handles time interrupt and updates the time counter
    registers: registers values
*/
void time_handler(registers_t* registers);

#endif