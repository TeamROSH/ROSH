#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "../IDT/reg_def.h"
#include "../memory/paging.h"

#define PROCESS_NAME 512

#define PROCESS_CREATED 0
#define PROCESS_RUNNING 1 
#define PROCESS_WAITING 2
#define PROCESS_READY 4
#define PROCESS_DEAD 8      
#define PROCESS_ZOMBI 16    //idle

#define NUM_MAX_PROCESS 1024    // there is a limited amount of pages


typedef struct process_context_block{
    char name[PROCESS_NAME];        // process name
    uint32_t pid;                   // process id
    registers_t reg;                // curr process reg values
    uint32_t process_state;          // the process state
    uint8_t is_kernel;              // kernel or user process
    page_table* pt                  // the process page table
    uint32_t* stack                 // stack base pointer
    uint32_t* stack_top             // stack top pointer
    uint32_t* data                  // data pointer
    uint32_t data_size              // data size
    uint32_t  stack_size            //  curr stack size
    }process_context_block;

#endif