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

void switch_endian(void* buffer, uint32_t bytes)
{
	uint8_t temp = 0;
	uint8_t* work_buffer = (uint8_t*)buffer;
	for (int i = 0; i < (bytes >> 1); i++)
	{
		temp = work_buffer[i];
		work_buffer[i] = work_buffer[bytes - i - 1];
		work_buffer[bytes - i - 1] = temp;
	}
}

uint32_t num_format_endian(void* pNum, uint32_t bytes)
{
	if (bytes == 2)
		return (*((uint16_t*)pNum) & 0xFF) << 8 | *((uint16_t*)pNum) >> 8;
	else if (bytes == 4)
	{
		return (*((uint32_t*)pNum) & 0xFF) << 24 | (*((uint32_t*)pNum) & 0xFF00) << 16 | (*((uint32_t*)pNum) & 0xFF000000) >> 24 | (*((uint32_t*)pNum) & 0xFF0000) >> 16;
	}
}