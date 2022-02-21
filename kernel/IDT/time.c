#include "time.h"

 uint32_t g_seconds_count = 0;
 uint32_t g_ms_counter = 0;
 callback_function g_scheduler;
 uint32_t last_call = 0;

void print_time_seconds();
void sleep(uint32_t sleep_ms);
void set_scheduler(callback_function scheduler);
uint32_t get_time();


void time_handler(registers_t* registers)
{
    static uint32_t i = 0;
    g_ms_counter += 55;
    g_seconds_count = g_ms_counter / 1000;


    i = g_ms_counter / 100;
    // cals the scheduler function every 200 ms 
    if(i != last_call && i % 10 == 0)
    {
		last_call = i;
        g_scheduler(registers);
    } 
}


void print_time_seconds()
{
    puts("curr time: ");
    puti(g_seconds_count);
    putc('\n');   
}

void sleep(uint32_t sleep_ms)
{
    // get curr miliseconds time 
    uint32_t curr_ms = g_ms_counter;

    // loop untile enough seconds waited
    while(curr_ms + sleep_ms > g_ms_counter){}
}

void set_scheduler(callback_function scheduler)
{
    g_scheduler = scheduler;
}

uint32_t get_time()
{
    return g_ms_counter;
}