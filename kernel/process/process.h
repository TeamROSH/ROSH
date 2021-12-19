#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "../IDT/reg_def.h"
#include "../memory/paging.h"
#include "../../libc/list.h"
#include "../../libc/memory.h"
#include "../heap.h"

#define PROCESS_NAME 512

#define PROCESS_CREATED 0
#define PROCESS_RUNNING 1 
#define PROCESS_WAITING 2
#define PROCESS_READY 4
#define PROCESS_DEAD 8      
#define PROCESS_ZOMBI 16    //idle

#define MAX_PROCESS_PAGES 20
#define MAX_PROCESS 1024    // there is a limited amount of pages


typedef struct process_context_block{
    char name[PROCESS_NAME];        // process name
    uint32_t pid;                   // process id
    registers_t reg;                // curr process reg values
    uint32_t process_state;          // the process state
    uint8_t is_kernel;              // kernel or user process
    uint32_t process_pages[MAX_PROCESS_PAGES];
    uint32_t stack_base;
    Heap process_heap;
    }process_context_block;

#endif

/*
    This function creates a process
    @param is_kernel: 1 if kernel 0 if user
    @returns the pcb of the new process 
*/
process_context_block* create_process(int is_kernel);

/*
    This function generates pid
    @returns the new pid
*/
int generate_pid();


/*
    This function initializes the pocess context block registes
    @param pcb: process_context_block pointer
*/
void initialize_process_regs(process_context_block* pcb);

/*
    This function will get the elf file and load it's  content into the process pages and  update the pcb registers
    @param pcb: the process context block
    @param file name: the elf file name 
*/
void load_process_code(process_context_block* pcb, char* file_name);

/*
    This function unmap the process pages and frees the process context block
    @param pcb: the process context block
*/
void kill_process(process_context_block* pcb);
