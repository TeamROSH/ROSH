#include "process.h"

extern Heap g_kernelHeap;
extern page_directory* g_page_directory;

int g_highest_pid = 1;
node* g_curr_process;

// holds the current process memory base for process memory allocations
uint32_t g_curr_process_base; 

process_context_block* create_process(int is_kernel);
int generate_pid();
void initialize_process_regs(process_context_block* pcb);
void load_process_code(process_context_block* pcb, char* file_name);


process_context_block* create_process(int is_kernel, char[PROCESS_NAME] process_name)
{
    process_context_block* pcb = heap_malloc(&g_kernelHeap, sizeof(process_context_block));

    pcb->is_kernel = is_kernel;
    pcb->pid = generate_pid();
    memcpy(pcb->name, process_name, PROCESS_NAME);
    initialize_process_regs(pcb);
    
    //mapping stack
    pcb->process_pages[1] = page_to_address(page_alloc());
    pcb->process_pages[0] = page_to_address(page_alloc());
    pcb->stack_base = pcb->process_pages[0];

    // allocating apace for heap
    for(int i =2; i < 5; i ++)
    {
        pcb->process_pages[i] = page_to_address(page_alloc());
    }
    
    // crating heap object 
    heap_init(pcb->process_heap, pcb->process_pages[2], PAGE_SIZE * 3);


    load_process_code(pcb, process_name);

    pcb->process_state = PROCESS_CREATED;

    return pcb;
}

int generate_pid()
{
    // if there is no more space for more processes 
    if(g_highest_pid == MAX_PROCESS)
    {
        return NULL;
    }

    // inc the pid counter
    g_highest_pid++;

    retrun g_highest_pid - 1;
}

void initialize_process_regs(process_context_block* pcb)
{
    pcb->reg.eax = 0;
    pcb->reg.ebx = 0;
    pcb->reg.ecx = 0;
    pcb->reg.edx = 0;
    pcb->reg.esi = 0;
    pcb->reg.edi = 0;
    pcb->reg.eflags = 518;
}

void load_process_code(process_context_block* pcb, char* file_name)
{
    // TODO
    // will be implemented when elf end file system will be implemented 
    // TODO
}