#include "paging.h"

//array of bytes each bit represents page
uint8_t g_pages_array[PAGES_COUNT];

page_directory* g_page_directory;

void allow_paging();
uint32_t page_to_address(uint32_t page_number);
uint32_t address_to_page(uint32_t address);
void page_map(page_directory* directory, uint32_t vadd, uint32_t padd, int flags);
void page_unmap(uint32_t vadd);
void update_pages_array(uint32_t page_num, int is_on);
uint32_t page_alloc();
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

void page_map(page_directory* directory, uint32_t vadd, uint32_t padd, int flags)
{
    uint32_t page_num= 0;
    uint32_t page_table_num = 0;
    page_table* pt;
    
    //getting page and page table number
    page_table_num = vadd >> 22;
	page_num = (vadd >> 12) & 0x3ff;

    //translating the page table number into addres 
    pt = (page_table*)page_to_address(PAGE_TABLES_START + page_table_num);
    
    //initializing pde according to the flags
    initialize_page_table_entry(&directory->directory_entries[page_table_num],
    (uint32_t)pt >> 12,
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

    //initializing the pte according to the flags
    initialize_page_table_entry(&pt->table_entries[page_num],
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

    //TODO: remember to set the page
}

void page_unmap(uint32_t vadd)
{
    uint32_t page_num= 0;
    uint32_t page_table_num = 0;
    page_table* pt;
    
    //getting page and page table number
    page_table_num = vadd >> 22;
	page_num = (vadd >> 12) & 0x3ff;

    //translating the page table number into addres 
    pt = (page_table*)page_to_address(PAGE_TABLES_START + page_table_num);
    initialize_page_table_entry(&pt->table_entries[page_num],
    0,
    0,
    0, 
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0);
}

uint32_t address_to_page(uint32_t address)
{
    return (address - PAGES_START_ADDR) / PAGE_SIZE;
}

uint32_t page_to_address(uint32_t page_number)
{
    return page_number * PAGE_SIZE + PAGES_START_ADDR;
}

void update_pages_array(uint32_t page_num, int is_on)
{
    //getting the byte index into the g_pages_array to be updated
    uint32_t page_byte_array = page_num / 8;
    //getting the bit in the page array byte index that will be updated
    uint32_t page_bit = page_byte_array % 8;

    //updating according to is_on
    g_pages_array[page_byte_array] = is_on == 1 ? g_pages_array[page_byte_array] | (1 << page_bit) : g_pages_array[page_byte_array] & ~(1 <<page_bit);
    
}

uint32_t page_alloc()
{

    uint8_t curr_bit = 0;
    for(int i = 0; i < PAGES_COUNT; i++)
    {
        //if there is a space for page to be mapped
        if(g_pages_array[i] != 0xFF)
        {
            //saving the 
            curr_bit= g_pages_array[i];
            for(int j = 0; j < 8; j++)
            {
                curr_bit =(1 << j);
                if((g_pages_array[i] & curr_bit) == 0)
                {
                    //updating the page_array 
                    update_pages_array(curr_bit * 8 + j, 1);

                    //returning the page num    
                    return curr_bit *8 +j;
                }

            }
        }
    }
}

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