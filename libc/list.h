#ifndef LIST_H
#define LIST_H
#include "../kernel/memory/heap.h"

typedef struct node
{
    node* next;
    node* prev;
    void* data;
}node;


typedef struct list
{
    node* head;
    node* tail;
    uint32_t size;
}list;

/*
    This function creates a list
    @retruns a pointer to the new list
*/
list* create_list();

node* insert_head(list* list, void* data);

#endif