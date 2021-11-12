#include "memory.h"

void memcpy(void* dst, void* src, uint32_t n)
{
	for (int i = 0; i < n; i++)
		*((char*)dst + i) = *((char*)src + i);
}