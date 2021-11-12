#include "heap.h"

void heap_init(Heap* heap, uint32_t base, uint32_t size)
{
	heap->base = base;
	heap->size = size;
	heap->nextFree = base;		// heap is empty so first free is base
	heap->head = NULL;			// empty pointers
	heap->tail = NULL;
}