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

/*
    This function loads the g_page_directory tale into the cr3 register
    directory: the page directory pointer 
*/
void load_directory_table(page_directory* directory);

/*
    This function maps page into physical address
    directory: pointer to the paging directory
    vadd: virtual address
    padd: physical address 
    flags: specify the flags that should be set
*/
void page_map(page_directory* directory, uint32_t vadd, uint32_t padd, int flags);

/*
    This function unmaps page from physical address
    vadd: virtual address
*/
void page_unmap(uint32_t vadd);

/*
    This function changes the cr0 register value in order to allow paging
*/
void allow_paging();

/*
    This function allocates page 
    retrun:page num
*/
uint32_t page_alloc();


/*
    This function updates the pages array due to page alloc or free
    page_num: the number of the page into the g_pages_array
    is_on: 0 if alloc 0 if free
*/
void update_pages_array(uint32_t page_num, int is_on);

/*
    This function translates page number to physical address
    page_number: page number
*/
uint32_t page_to_address(uint32_t page_number);


/*
    This function translates physical address to page number
    address: page physical address
*/
uint32_t address_to_page(uint32_t address);

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
uint32_t address,
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