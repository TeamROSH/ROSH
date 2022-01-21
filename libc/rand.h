#ifndef RAND_H
#define RAND_H

#include <stdint.h>

/*
    This function generates a 32 bit rand value
    returns the generated rand value
*/
uint32_t rand();


/*
    This function sets the seed
    @param new_seed: the new seed value
*/
void srand(uint32_t new_seed);

#endif