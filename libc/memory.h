#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"

/*
	copy memory between addresses
	@param dst: destination address
	@param src: source address
	@param n: number of bytes to copy
*/
void memcpy(void* dst, void* src, uint32_t n);

#endif