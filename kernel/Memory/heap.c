#include "heap.h"

void heap_init(Heap* heap, uint32_t base, uint32_t size)
{
	heap->base = base;
	heap->size = size;
	heap->nextFree = base;		// heap is empty so first free is base
	heap->head = NULL;			// empty pointers
	heap->tail = NULL;
}

void heap_pushNode(Heap* heap, HeapNode* node)
{
	node->next = NULL;			// clear next since added to the end
	node->prev = head->tail;	// add to end
	if (heap->head == NULL)		// if heap empty
		heap->head = node;			// add to beginning
	else						// if heap not empty
		heap->tail->next = node;	// set prev's next to node
	heap->tail = node;			// add to end
}

void heap_popNode(Heap* heap)
{
	if (heap->head == heap->tail)		// if first is last - one left
	{
		heap->head = NULL;					// clear heap
		heap->tail = NULL;
	}
	else
	{
		heap->tail = heap->tail->prev;	// clear last
        heap->tail->next = NULL;		// make prev the new last
	}
}