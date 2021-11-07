#include "time.h"

 uint64_t g_seconds_count = 0;

void print_time_seconds();


void time_handler(registers_t* registers)
{
    static unsigned int time = 0;

    if(time % 1000 == 0)
    {
        g_seconds_count++;

    }

    time += 55;
}


void print_time_seconds()
{
    puts("curr time: ");
    puti(g_seconds_count);
    putc('\n');   
}