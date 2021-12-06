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

/*
    This function creates a node
    @param data the new node data
    @retruns the function retruns the new node
*/
node* create_node(void* data);

/*
    This function creates a new node and insets it at the head
    @param list: the list the new node will be added to
    @param data: the data of the new node
    @retruns a pointer to the new node 
*/
node* insert_head(list* list, void* data);

/*
    This function creates a new node and insets it at the tail
    @param list: the list the new node will be added to
    @param data: the data of the new node
    @retruns a pointer to the new node 
*/
node* insert_tail(list* list, void* data);

#endif