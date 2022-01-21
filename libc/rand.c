#include "rand.h"

static uint32_t seed = 1;

uint32_t rand()
{
    seed  = seed * 1232404134 + 54321;
    return (uint32_t)( seed / 6536) % 32768
}
