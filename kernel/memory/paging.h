#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>
#include <stdbool.h>

#include "memorylayout.h"
#include "../../libc/memory.h"
#include "../../libc/rand.h"


//TODO DELETE LATER
#include "../../libc/screen.h"
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

typedef struct{
    page_table_entry table_entries[1024];
}page_table __attribute__((aligned(0x1000)));

typedef struct{
    page_table_entry directory_entries[1024];
} page_directory __attribute__((aligned(0x1000)));

void initialize_paging();
void allow_paging();
void load_directory_table(page_directory* directory);
uint32_t page_to_address(uint32_t page_number);
uint32_t address_to_page(uint32_t address);
void page_map(page_directory* directory, uint32_t vadd, uint32_t padd, int flags);
void page_unmap(uint32_t vadd);
void update_pages_array(uint32_t page_num, int is_on);
uint32_t page_alloc();
uint32_t rand_page_alloc(uint32_t num_of_pages);
uint32_t check_bits_in_byte(uint8_t byte, int num_of_bits);
void page_free(uint32_t page_num);
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