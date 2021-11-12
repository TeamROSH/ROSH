#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct HeapNode
{
	uint8_t free;		// is free node?
	HeapNode* next;		// next node of heap
	HeapNode* prev;		// previous node of heap
	uint32_t dataSize;	// node data size
	void* data;			// the data
} HeapNode;


typedef struct Heap
{
	uint32_t base;		// base address for heap
	uint32_t nextFree;	// next free address
	uint32_t size;		// size of heap
	HeapNode* head;		// first node
	HeapNode* tail;		// last node
} Heap;

/*
	init the heap
	@param heap: pointer to the heap struct
	@param base: base address for heap
	@param size: size of heap
*/
void heap_init(Heap* heap, uint32_t base, uint32_t size);

#endif