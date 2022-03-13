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

/*
	switch little and big endian
	@param buffer: the buffer to switch
	@param bytes: the buffer size in bytes
*/
void switch_endian(void* buffer, uint32_t bytes);

/*
	switch little and big endian
	@param pNum: the num pointer
	@param bytes: the buffer size in bytes (2/4)
	@returns formated num
*/
uint32_t num_format_endian(void* pNum, uint32_t bytes);

#endif