#include "memory.h"

void memcpy(void* dst, const void* src, uint32_t n)
{
	for (int i = 0; i < n; i++)
		*((char*)dst + i) = *((char*)src + i);
}

void memset(void* dst, const uint8_t n, uint32_t size)
{
	for (int i = 0; i < size; i++)
		*((uint8_t*)dst + i) = n;
}