#include "heap.h"
#include "memorylayout.h"
#include "../../libc/memory.h"
#define FALSE 0
#define TRUE !FALSE
#define CHECKSUM 0x552F60B2
#define NULL 0
#define min(a, b) ((a) < (b) ? (a) : (b))

Heap g_kernelHeap;

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
	node->prev = heap->tail;	// add to end
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
		heap->nextFree = heap->base;		// reset next free
	}
	else
	{
		heap->nextFree = (uint32_t)(heap->tail);	// reset next free
		heap->tail = heap->tail->prev;	// clear last
        heap->tail->next = NULL;		// make prev the new last
	}
}

void* heap_malloc(Heap* heap, uint32_t size)
{
	int physicalSize = size + sizeof(HeapNode);		// get total size of node + data
	HeapNode* node;
	if (heap->nextFree + physicalSize <= heap->base + heap->size)		// if heap has room at the end
	{
		node = (HeapNode*)heap->nextFree;		// allocate free space for the node
		node->free = FALSE;						// node is not free
		node->dataSize = size;					// set data size
		node->checksum = CHECKSUM;				// set checksum
		node->data = (void*)(heap->nextFree + sizeof(HeapNode));		// allocate free space for the data

		heap_pushNode(heap, node);				// push the node to the heap
		heap->nextFree += physicalSize;			// next free address is now after the node
		return node->data;						// return the node data to the user
	}
	else		// if no room at the end - search for room in the middle
	{
		node = heap->head;
		while (node != NULL)		// iterate over heap
		{
			if (node->free && node->dataSize >= size)		// if free and has room for data
			{
				node->free = FALSE;			// save for us
				return node->data;			// this node's data was already defined
			}
			else
				node = node->next;			// continue iterate if not found
		}
	}
	return NULL;		// if got here - no free memory was found
}

void heap_free(Heap* heap, void* addr)
{
	if ((uint32_t)addr < heap->base || (uint32_t)addr > heap->base + heap->size)		// if not in heap
		return;

	HeapNode* node = (HeapNode*)(addr - sizeof(HeapNode));		// get node from data pointer
	if (node->checksum != CHECKSUM)		// check checksum to prevent buffer overflow
		return;

	if (node == heap->tail)		// if last
		heap_popNode(heap);			// remove last
	else						// if not last
		node->free = TRUE;			// mark as free
}

void* heap_realloc(Heap* heap, void* addr, uint32_t size)
{
	if ((uint32_t)addr < heap->base || (uint32_t)addr > heap->base + heap->size)		// if not in heap
		return heap_malloc(heap, size);

	HeapNode* node = (HeapNode*)(addr - sizeof(HeapNode));		// get node from data pointer
	if (node->checksum != CHECKSUM)		// check checksum to prevent buffer overflow
		return heap_malloc(heap, size);
	
	if (node->free == TRUE)		// if no node taken already
		return heap_malloc(heap, size);

	if (node->dataSize >= size)		// if node has already enough space
		return node->data;				// return same node
	else
	{
		void* data = heap_malloc(heap, size);		// allocate new node
		if (data == NULL)			// if failed allocating
			return NULL;
		memcpy(data, node->data, min(node->dataSize, size));		// copy memory
		heap_free(heap, node->data);		// free old memory
		return data;		// return new address
	}
}

void initKernelHeap()
{
	heap_init(&g_kernelHeap, KERNEL_HEAP_START, KERNEL_HEAP_SIZE * PAGE_SIZE);
}

void* kmalloc(uint32_t size)
{
	return heap_malloc(&g_kernelHeap, size);
}

void kfree(void* addr)
{
	heap_free(&g_kernelHeap, addr);
}

void* krealloc(void* ptr, uint32_t size)
{
	heap_realloc(&g_kernelHeap, ptr, size);
}