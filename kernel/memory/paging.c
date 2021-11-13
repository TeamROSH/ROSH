#include "paging.h"

uint8_t g_pages_array[PAGES_COUNT];
page_directory* g_page_directory;

void allow_paging();
uint page_to_address(uint page_number);
uint address_to_page(uint address);
void page_map(page_directory* directory, uint vadd, uint padd, int flags);
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

void page_map(page_directory* directory, uint vadd, uint padd, int flags)
{
    uint page_num= 0;
    uint page_table_num = 0;
    page_table* pt

    page_table_num = vadd >> 22;
	page_num = (addr >> 12) & 0x3ff;

    pt = page_to_address(PAGE_TABLES_START + page_table_num)
    initialize_page_table_entry(&directory->page_directory_entery[page_table_num],
    (uint)pt >> 12,
    1,
    (flags & PAGE_FLAG_READWRITE) ? 1 : 0, 
    (flags & PAGE_FLAG_KERNEL) ? 0 : 1,
    0,
    0,
    0,
    0,
    0,
    0,
    0);

    initialize_page_table_entry(&pt->page_table_entry[page_num],
    padd >> 12,
    1,
    (flags & PAGE_FLAG_READWRITE) ? 1 : 0, 
    (flags & PAGE_FLAG_KERNEL) ? 0 : 1,
    0,
    0,
    0,
    0,
    0,
    0,
    0);
}

uint address_to_page(uint address)
{
    return (address - PAGES_START_ADDR) / PAGE_SIZE;
}

uint page_to_address(uint page_number)
{
    return page_number * PAGE_SIZE + PAGES_START_ADDR;
}

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
uint8_t avl)
{
    table_entry->address = address;
    table_entry->present = present;
    table_entry->rw = rw;
    table_entry->us = us;
    table_entry->pwt = pwt;
    table_entry->pcd = pcd;
    table_entry->accessed = accessed;
    table_entry->dirty = dirty;
    table_entry->pat = pat;
    table_entry->global = global;
    table_entry->avl = avl;
}

void allow_paging()
{
	asm("movl %cr0, %eax");
    //enabling paging
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
}