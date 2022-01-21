#ifndef RAND_H
#define RAND_H

#include <stdint.h>

uint32_t rand();

void srand(uint32_t new_seed);

#endif