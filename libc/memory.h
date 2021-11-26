#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"

/*
	copy memory between addresses
	@param dst: destination address
	@param src: source address
	@param n: number of bytes to copy
*/
void memcpy(void* dst, const void* src, uint32_t n);

/*
	set size of memory to n value
	@param dst: destination address
	@param n: value to set
	@param size: number of bytes to set
*/
void memset(void* dst, const uint8_t n, uint32_t size);

#endif