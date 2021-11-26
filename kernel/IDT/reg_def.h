#ifndef REG_DEF_H
#define REG_DEF_H

typedef struct {
    uint32_t es;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t interrupt_num, err_code;             
    uint32_t eip, cs, eflags, user_esp, ss;           //cpu pushes

}registers_t;

#endif