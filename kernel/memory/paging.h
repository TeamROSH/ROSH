#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>

#include "memorylayout.h"

//specifies flags for mapped page
#define PAGE_FLAG_USER        0
#define PAGE_FLAG_KERNEL      1
#define PAGE_FLAG_EXISTS      0
#define PAGE_FLAG_ALLOC       2
#define PAGE_FLAG_READONLY    0
#define PAGE_FLAG_READWRITE   4
#define PAGE_FLAG_NOCLEAR     0
#define PAGE_FLAG_CLEAR       8


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
    uint address : 20;      // page physical address
}__attribute__((packed)) page_table_entry;

typedef struct page_table_entry page_directory_entery;

typedef struct{
    page_table_entry[1024];
}page_table;

typedef struct{
    page_directory_entery[1024];
} page_directory;

/*
    This function initializes the paging
*/
void initialize_paging();


/*
    This function maps virtual address to phisycal address
    directory: pointer to the paging directory
    vadd: virtual address
    padd: physical address 
    flags: specify the flags that should be set
*/
void page_map(page_directory* directory, uint vadd, uint padd, int flags);

/*
    This function changes the cr0 register value in order to allow paging
*/
void allow_paging();


/*
    This function translates page number to physical address
    page_number: page number
*/
uint page_to_address(uint page_number);


/*
    This function translates physical address to page number
    address: page physical address
*/
uint address_to_page(uint address);

/*
    This function initializes the pte
    table_entry: pointer to the pte
    address: page physical address
    present: is present
    rw: read write or read only
    us: user or kernel
    pwt:    nulled
    pcd:    nulled
    accessed:   nulled
    dirty:  nulled
    pat:    nulled
    global: nulled
    avl:    nulled
*/
void initialize_page_table_entry(page_table_entry* table_entry,
uint address,
uint8_t present,    
uint8_t rw,         
uint8_t us,        
uint8_t pwt,        
uint8_t pcd,       
uint8_t accessed,
uint8_t dirty,
uint8_t pat,      
uint8_t global,     
uint8_t avl);

#endif