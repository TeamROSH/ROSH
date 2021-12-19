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


process_context_block* create_process(int is_kernel, char[PROCESS_NAME] process_name)
{
    process_context_block* pcb = heap_malloc(&g_kernelHeap, sizeof(process_context_block));

    pcb->is_kernel = is_kernel;
    pcb->pid = generate_pid();
    memcpy(pcb->name, process_name, PROCESS_NAME);
    pcb->process_base = g_curr_process;
    pcb->process_top = g_curr_process + PAGE_SIZE * 20;
    pcb->reg.esp = 0x100000;
    
    //mapping stack
    pcb->stack = 0x100000;
    uint32_t* stackPhy = page_to_address(page_alloc());
    page_map(g_page_directory, stack, stackPhy, PAGE_FLAG_READWRITE | PAGE_FLAG_EXISTS | is_kernel);

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