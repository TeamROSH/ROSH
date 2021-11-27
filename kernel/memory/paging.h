#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>

#include "memorylayout.h"
#include "../../libc/memory.h"

//specifies flags for mapped page
#define PAGE_FLAG_USER        0
#define PAGE_FLAG_KERNEL      1
#define PAGE_FLAG_EXISTS      0
#define PAGE_FLAG_ALLOC       2
#define PAGE_FLAG_READONLY    0
#define PAGE_FLAG_READWRITE   4
#define PAGE_FLAG_NOCLEAR     0
#define PAGE_FLAG_CLEAR       8

#define BITS_IN_BYTE 8

typedef struct {
    uint8_t present : 1;    //one bit is entry present
    uint8_t rw : 1;         // read write or read only
    uint8_t us : 1;         // user or seupervisor mode
    uint8_t pwt : 1;        //write through if set, write back if not
    uint8_t pcd : 1;        // is page cashed
    uint8_t accessed : 1;   //if read during virtual address translation
    uint8_t dirty : 1;      // whether page has been written to
    uint8_t pat : 1;        // indicates memory caching type
    uint8_t global : 1;     // whether TLB coresponding to the page should be invalidate
    uint8_t avl : 3;        // available
    uint32_t address : 20;      // page physical address
}__attribute__((packed)) page_table_entry;

typedef struct page_table_entry page_directory_entery;

typedef struct{
    page_table_entry table_entries[1024];
}page_table __attribute__((aligned(0x1000)));

typedef struct{
    page_table_entry directory_entries[1024];
} page_directory __attribute__((aligned(0x1000)));

/*
    This function initializes the paging
*/
void initialize_paging();

#endif