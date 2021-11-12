#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef struct HeapNode
{
	uint8_t free;		// is free node?
	HeapNode* next;		// next node of heap
	HeapNode* prev;		// previous node of heap
	uint32_t dataSize;	// node data size
	uint32_t checksum;	// security. must always be CHECKSUM const
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

/*
	push heap node to the end of the heap
	@param heap: pointer to the heap struct
	@param node: the node to push
*/
void heap_pushNode(Heap* heap, HeapNode* node);

/*
	remove the last node
	@param heap: pointer to the heap struct
*/
void heap_popNode(Heap* heap);

/*
	allocate <size> to the heap
	@param heap: pointer to the heap struct
	@param size: the size of the data to add
	@returns pointer to the data
*/
void* heap_malloc(Heap* heap, uint32_t size);

/*
	free address in heap
	@param heap: pointer to the heap struct
	@param addr: address to free
*/
void heap_free(Heap* heap, void* addr);

/*
	init the kernel heap
*/
void initKernelHeap();

#endif