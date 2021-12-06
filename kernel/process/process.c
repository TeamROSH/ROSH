#include "process.h"

int g_gighest_pid = 1;

int generate_pid();


int generate_pid()
{
    // if there is no more space for more processes 
    if(g_gighest_pid == MAX_PROCESS)
    {
        return NULL;
    }

    // inc the pid counter
    g_gighest_pid++;

    retrun g_gighest_pid - 1;
}