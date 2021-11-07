#include "time.h"

 uint64_t g_seconds_count = 0;

void time_handler(registers_t* registers)
{
    //static uint64_t time = 0;

    //if(time % 1000 == 0)
    //{
        g_seconds_count++;

    //}

    //time += 55;
}
