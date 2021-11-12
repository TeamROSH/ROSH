#include "memory.h"

void memcpy(void* dst, void* src, size_t n)
{
	for (int i = 0; i < n; i++)
		*(dst + i) = *(src + i);
}