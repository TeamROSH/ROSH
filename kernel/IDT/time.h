#ifndef TIME_H
#define TIME_H
#include <stdint.h>
#include "../../libc/screen.h"
#include "reg_def.h"

typedef void (*interrupt_handler)(registers_t* registers);


/*
    This function handles time interrupt and updates the time counter
    registers: registers values
*/
void time_handler(registers_t* registers);

/*
    This function prints the curr time in seconds
*/
void print_time_seconds();

#endif