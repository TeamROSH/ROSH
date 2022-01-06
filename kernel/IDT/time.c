#include "time.h"

 uint64_t g_seconds_count = 0;
 uint64_t g_ms_counter = 0;

void print_time_seconds();

void sleep(uint64_t sleep_ms);

void time_handler(registers_t* registers)
{

    g_ms_counter += 55;
    g_seconds_count = g_ms_counter / 1000;
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