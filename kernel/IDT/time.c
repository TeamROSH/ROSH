#include "time.h"

 uint64_t g_seconds_count = 0;
 uint64_t g_ms_counter = 0;
 callback_function g_scheduler;

void print_time_seconds();
void sleep(uint64_t sleep_ms);
void set_scheduler(callback_function scheduler);


void time_handler(registers_t* registers)
{
    static uint64_t i = 0;
    g_ms_counter += 55;
    g_seconds_count = g_ms_counter / 1000;


    i = g_ms_counter / 100;
    // cals the scheduler function every 200 ms 
    if(i % 2 == 0)
    {
        g_scheduler();
    } 
}


void print_time_seconds()
{
    puts("curr time: ");
    puti(g_seconds_count);
    putc('\n');   
}

void sleep(uint64_t sleep_ms)
{
    // get curr miliseconds time 
    uint64_t curr_ms = g_ms_counter;

    // loop untile enough seconds waited
    while(curr_ms + sleep_ms > g_ms_counter){}
}

void set_scheduler(callback_function scheduler)
{
    g_scheduler = scheduler;
}