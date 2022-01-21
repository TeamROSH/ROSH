#ifndef TIME_H
#define TIME_H
#include <stdint.h>
#include "../../libc/screen.h"
#include "reg_def.h"
#define NULL 0

typedef void (*interrupt_handler)(registers_t* registers);
typedef void (*callback_function)(registers_t* registers);


/*
    This function handles time interrupt and updates the time counter
    registers: registers values
*/
void time_handler(registers_t* registers);

/*
    This function prints the curr time in seconds
*/
void print_time_seconds();

/*
    this function sleeps for specific amount of miliseconds
    @param sleep_ms: the wanted sleep time
*/
void sleep(uint32_t sleep_ms);

/*
    This function sets the scheduler function to be called every 200 miliseconds 
    @parma scheduler: the scheduler function 
*/
void set_scheduler(callback_function scheduler);

/*
    This function returns the curr time 
    returns the currtime in ms
*/
uint32_t get_time();
#endif