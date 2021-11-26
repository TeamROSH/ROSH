#include "paging.h"

//array of bytes each bit represents page
uint8_t g_pages_array[PAGES_COUNT];

page_directory* g_page_directory;

void initialize_paging();
void allow_paging();
void load_directory_table(page_directory* directory);
uint32_t page_to_address(uint32_t page_number);
uint32_t address_to_page(uint32_t address);
void page_map(page_directory* directory, uint64_t vadd, uint64_t padd, int flags);
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

void initialize_paging()
{
    int i = 0;

    for(i = 0; i < PAGES_COUNT; i++)
    {
        g_pages_array[i] = 0;
    }

    //mapping page for the page directory
    update_pages_array(PAGE_DIRECTORY_START,1);
    
    //mappign pages for the page tale
    for (i = 0; i< PAGE_TABLE_COUNT; i++)
    {
        update_pages_array(PAGE_TABLES_START + i, 1);
    }

    //converting the page directory into address and initializing the page directory array
    g_page_directory = (page_directory*)page_to_address(PAGE_DIRECTORY_START);

    //setting all the page directory entries as not present
    for(i = 0; i < PAGE_TABLE_COUNT; i++)
    {
        initialize_page_table_entry(&g_page_directory->directory_entries[i], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    // load page directory
    load_directory_table(g_page_directory);

    //mapping kernel code as read only memory
    for( i = 0; i < KERNEL_SOURCE_SIZE; i++)
    {
        page_map(g_page_directory, KERNEL_START_ADDR + i * PAGE_SIZE, KERNEL_START_ADDR + i * PAGE_SIZE, PAGE_FLAG_KERNEL | PAGE_FLAG_READONLY);
    }

    //mapping kernel stack as readwrite memory
    for(i = 0;i < KERNEL_STACK_SIZE; i++)
    {
        page_map(g_page_directory, KERNEL_STACK_START_ADDR + i * PAGE_SIZE, KERNEL_STACK_START_ADDR + i *PAGE_SIZE, PAGE_FLAG_KERNEL | PAGE_FLAG_READWRITE);
    }

    for(i = 0; i < KERNEL_HEAP_SIZE; i++)
    {
        page_map(g_page_directory, KERNEL_HEAP_START + i * PAGE_SIZE, KERNEL_HEAP_START + i * PAGE_SIZE, PAGE_FLAG_READWRITE | PAGE_FLAG_KERNEL);
    }

    //mapping the video memory into the physical address
    page_map(g_page_directory, VIDEO_MEM_START, VIDEO_MEM_PHYSICAL_ADDR, PAGE_FLAG_READWRITE | PAGE_FLAG_KERNEL);
    
    //mapping the page tables and page directory
    for(i= 0; i < PAGE_TABLE_COUNT; i++)
    {
        page_map(g_page_directory, page_to_address(i), page_to_address(i), PAGE_FLAG_READWRITE | PAGE_FLAG_USER);
    }
    
    //allow_paging();
}

void page_map(page_directory* directory, uint64_t vadd, uint64_t padd, int flags)
{
    int64_t page_num= 0;
    int64_t page_table_num = 0;
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

    //updatin the page array
    update_pages_array(address_to_page(padd), 1);
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
                if(!(g_pages_array[i] & curr_bit))
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

void load_directory_table(page_directory* directory)
{
        asm volatile("mov 0, %eax;");
        asm volatile("mov %%eax, %%cr3" : : "a" (directory));
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