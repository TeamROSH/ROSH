#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>

typedef struct {
    uint8_t present : 1;    //one bit is entry present
    uint8_t rw : 1;         // read write or read only
    uint8_t us : 1;         // user orseupervisor mode
    uint8_t pwt : 1;        //should be null
    uint8_t pcd : 1;        // should be nulled
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t pat : 1;        // nulled
    uint8_t global : 1;     // nulled
    uint8_t avl : 3;        // nulled
    uint address : 20; 
}__attribute__((packed)) page_table_entry;

typedef struct page_table_entry page_directory_entery;

typedef struct{
    page_table_entry[1024];
}page_table;

typedef struct{
    page_directory_entery[1024];
} page_directory;

#endif