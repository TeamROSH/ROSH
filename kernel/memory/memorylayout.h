#ifndef MEMORYLAYOUT_H
#define MEMORYLAYOUT_h

#define PAGES_COUNT 1024*128
#define PAGES_START_ADDR 0x100000
#define PAGE_SIZE 0x1000

//sizes in pages 
#define PAGE_DIRECTORY_START 0
#define PAGE_TABLES_START 1
#define PAGE_TABLE_COUNT 1024

#define USER_PAGES_START PAGE_TABLES_START + PAGE_TABLE_COUNT 
#define KERNEL_START_ADDR 0x6400000 
#define KERNEL_SOURCE_SIZE 0x100 
#define KERNEL_STACK_START_ADDR KERNEL_START_ADDR + 256 * PAGE_SIZE
#define KERNEL_STACK_SIZE 4

#define VIDEO_MEM_PHYSICAL_ADDR 0xB8000

#define KERNEL_HEAP_START KERNEL_STACK_START_ADDR + PAGE_SIZE * KERNEL_STACK_SIZE
#define KERNEL_HEAP_SIZE 1024 * 4 

#define USER_MODE_START 0x7530000 // 32000 * PAGE_SIZE
#define USER_MODE_END 0xffff0000
#define USER_MODE_SIZE (USER_MODE_END- USER_MODE_START) / PAGE_SIZE 

#define USER_HEAP_START USER_MODE_START + USER_MODE_SIZE
#define USER_HEAP_SIZE KERNEL_HEAP_SIZE

#define VIDEO_MEM_START USER_HEAP_START + USER_HEAP_SIZE * PAGE_SIZE

#endif