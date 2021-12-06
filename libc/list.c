#include "list.h"


list* create_list();
node* insert_head(list* list, void* data);

list* create_list()
{
    list* new_list = (list*)heap_malloc(&g_kernelHeap, sizeof(list));
    new_list->size = 0;
    new_list->tail = NULL;
    new_list->head = NULL;
    return new_list;
}

node* insert_head(list* list, void* data)
{
    node* new_node = (node*)heap_malloc(&g_kernelHeap, sizeof(node));
    // if no more kernel heap space
    if(new_node == NULL)
    {
        retrun NULL;
    }

    // assigning data
    new_node->data = data;

    //
    list->head->next = new_node;
    list->head = new_node;
    list->size = list->size + 1;
    
}