#include "process.h"
#include "../GDT/tss.h"

#define hlt() asm volatile("hlt")
#define cli() asm volatile("cli")

extern page_directory* g_page_directory;

int g_highest_pid = 1;
process_context_block* g_curr_process; 

list* g_process_list;
list* g_ready_processes_list;
uint32_t g_curr_stack = 0;
uint8_t g_mutex = 0;

process_context_block* create_process(int is_kernel, char* process_name);
int generate_pid();
void initialize_process_regs(process_context_block* pcb);
void load_process_code(process_context_block* pcb, char* file_name);
void kill_process(process_context_block* pcb);
void process_scheduler(registers_t* registers);
void process_init();
int context_switch(process_context_block* next_process);
void save_registers(process_context_block* pcb, registers_t* registers);
process_context_block* init_usermain_process();
void init_process_stack(process_context_block* pcb);

extern void context_jump(uint32_t ptr);
extern void usermode(void);


process_context_block* init_usermain_process()
{
	process_context_block* pcb = (process_context_block*)kmalloc(sizeof(process_context_block));
	pcb->is_kernel = 0;
    pcb->pid = generate_pid();
	memcpy(pcb->name, "usermain", PROCESS_NAME);
	initialize_process_regs(pcb);

	pcb->process_pages[0] = USER_STACK_START_ADDR;
	pcb->stack_base = PROCESS_STACK + PROCESS_STACK_SIZE * PAGE_SIZE - 4;

	pcb->process_pages[2] = USER_HEAP_START;
	pcb->process_pages[5] = USER_MODE_START;

	pcb->reg.eip = pcb->process_pages[5];
	pcb->process_state = PROCESS_RUNNING;

    // adding the process to the processes list
    insert_head(g_process_list, pcb);

	g_curr_process = pcb;

    return pcb;
}


process_context_block* create_process(int is_kernel, char* process_name)
{
    process_context_block* pcb = (process_context_block*)kmalloc(sizeof(process_context_block));

    pcb->is_kernel = is_kernel;
    pcb->pid = generate_pid();
    memcpy(pcb->name, process_name, PROCESS_NAME);
    initialize_process_regs(pcb);

	for (int i = 0; i < 20; i++)		// zero all pages
	{
		pcb->process_pages[i] = -1;
	}

    //mapping stack
    pcb->process_pages[0] = page_to_address(rand_page_alloc(2));
    pcb->process_pages[1] = pcb->process_pages[0] + PAGE_SIZE;
    pcb->stack_base = PROCESS_STACK + PROCESS_STACK_SIZE * PAGE_SIZE;
	pcb->reg.esp = pcb->process_pages[1] + PAGE_SIZE - 4;

    // allocating apace for heap
    pcb->process_pages[2] = page_to_address(rand_page_alloc(3));
	pcb->process_pages[3] = pcb->process_pages[2] + PAGE_SIZE;
	pcb->process_pages[4] = pcb->process_pages[3] + PAGE_SIZE;;
    
    // crating heap object 
    heap_init(&(pcb->process_heap), pcb->process_pages[2], PAGE_SIZE * 3);

    load_process_code(pcb, process_name);
	init_process_stack(pcb);

	for (int i = 0; i < 20; i++)
	{
		if (pcb->process_pages[i] != -1)
		{
			page_map(g_page_directory, address_to_page(pcb->process_pages[i]), address_to_page(pcb->process_pages[i]), PAGE_FLAG_USER | PAGE_FLAG_READWRITE);
		}
	}

    pcb->process_state = PROCESS_READY;

    // adding the process to the processes list
    insert_head(g_process_list, pcb);
    insert_tail(g_ready_processes_list, pcb);

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

    return g_highest_pid - 1;
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
	pcb->reg.cs = 0x18 | 3;
	pcb->reg.es = 0x20 | 3;
}

void init_process_stack(process_context_block* pcb)
{
	pcb->reg.esp -= 64;
	*((uint32_t*)(pcb->reg.esp) + 15) = pcb->reg.es;
	*((uint32_t*)(pcb->reg.esp) + 14) = pcb->reg.esp + 64;
	*((uint32_t*)(pcb->reg.esp) + 13) = pcb->reg.eflags;
	*((uint32_t*)(pcb->reg.esp) + 12) = pcb->reg.cs;
	*((uint32_t*)(pcb->reg.esp) + 11) = pcb->reg.eip;
	*((uint32_t*)(pcb->reg.esp) + 10) = 0;
	*((uint32_t*)(pcb->reg.esp) + 9) = 0;
	*((uint32_t*)(pcb->reg.esp) + 8) = pcb->reg.eax;
	*((uint32_t*)(pcb->reg.esp) + 7) = pcb->reg.ecx;
	*((uint32_t*)(pcb->reg.esp) + 6) = pcb->reg.edx;
	*((uint32_t*)(pcb->reg.esp) + 5) = pcb->reg.ebx;
	*((uint32_t*)(pcb->reg.esp) + 4) = pcb->reg.esp + 64;
	*((uint32_t*)(pcb->reg.esp) + 3) = pcb->reg.ebp;
	*((uint32_t*)(pcb->reg.esp) + 2) = pcb->reg.esi;
	*((uint32_t*)(pcb->reg.esp) + 1) = pcb->reg.edi;
	*((uint32_t*)(pcb->reg.esp) + 0) = pcb->reg.es;
}

void load_process_code(process_context_block* pcb, char* file_name)
{
    int size = file_size(file_name) + 1;
	char* b64 = (char*)kmalloc(size);
	read_file(file_name, b64);		// read file
	b64[size - 1] = 0;
	size = calc_base64_output(b64);
	char* code = (char*)kmalloc(size);		// decode file
	base64_decode(b64, code);
	kfree(b64);

	for (int i = 0; i < size / PAGE_SIZE + 1; i++)
	{
		// only works if codeis 
		pcb->process_pages[i + 5] = page_to_address(rand_page_alloc(1));
		memcpy((void*)pcb->process_pages[i + 5], code + i * PAGE_SIZE, (i == size / PAGE_SIZE) ? (size % PAGE_SIZE) : PAGE_SIZE);
	}
	kfree(code);

	pcb->reg.eip = pcb->process_pages[5];
}

void kill_process(process_context_block* pcb)
{

    // free stack and heap
    for(int i = 0; i < 5; i++)
    {
        memset((void*)pcb->process_pages[i], 0, PAGE_SIZE);
		page_free(address_to_page(pcb->process_pages[i]));
    }
	delete_node_by_data(g_process_list, pcb);
	delete_node_by_data(g_ready_processes_list, pcb);

    kfree(pcb);
}

void process_scheduler(registers_t* registers)
{
	if (g_mutex)
		return;
    node* next_process;
    for (next_process = g_ready_processes_list->head; next_process != NULL; next_process = next_process->next)
	{
		process_context_block* proc = ((process_context_block*)next_process->data);
		if (proc->process_state == PROCESS_READY)
		{
			// saving the old process registers
			save_registers(g_curr_process, registers);
			g_curr_process->reg.esp = (uint32_t)registers;

			// pushing the current process to the list head
			insert_tail(g_ready_processes_list, g_curr_process);
			g_curr_process->process_state = PROCESS_READY;

			delete_node_by_data(g_ready_processes_list, proc);

			// context switching to the next process
			context_switch(proc);
		}
	}
}

void process_init()
{
    //initilizing the processes lists 
    g_process_list = create_list();
    g_ready_processes_list = create_list();

	// initializing the first process usermain
    process_context_block* usermain = init_usermain_process();

	// initializing the scheduler
    set_scheduler((callback_function)process_scheduler);

	// ***	Create first process before this line	***
	set_interrupt(32, time_handler);

	usermode();
}

int context_switch(process_context_block* next_process)
{
	if (next_process->stack_base == PROCESS_STACK + PROCESS_STACK_SIZE * PAGE_SIZE)
	{
		uint32_t min = PROCESS_STACK + PROCESS_STACK_SIZE * PAGE_SIZE - 4;
		node* loop_process;
		for (loop_process = g_process_list->head; loop_process != NULL; loop_process = loop_process->next)		// find min stack base
		{
			process_context_block* proc = ((process_context_block*)loop_process->data);
			min = proc->stack_base < min ? proc->stack_base : min;
		}
		next_process->stack_base = min - PAGE_SIZE;
	}
	g_curr_stack = next_process->stack_base;

    next_process->process_state = PROCESS_RUNNING;
	g_curr_process = next_process;

	set_kernel_stack(g_curr_stack);
	context_jump(next_process->reg.esp);
}

void save_registers(process_context_block* pcb, registers_t* registers)
{
    // copying the updated registers vlues into the process pcb
    memcpy(&(pcb->reg), registers, sizeof(registers_t));
}

void new_process(char* process_name)
{
	process_context_block* process = create_process(0, process_name);
}

void kill_running_process()
{
	if (g_curr_process->pid == 1)		// if usermain process dead
	{
		cli();		// hlt the cpu
		hlt();
	}
	else
	{
		kill_process(g_curr_process);
		node* next_process;
		for (next_process = g_ready_processes_list->head; next_process != NULL; next_process = next_process->next)
		{
			process_context_block* proc = ((process_context_block*)next_process->data);
			if (proc->process_state == PROCESS_READY)
			{
				delete_node_by_data(g_ready_processes_list, proc);
				context_switch(proc);
			}
		}
	}
}

void lock_mutex()
{
	g_mutex = 1;
}

void release_mutex()
{
	g_mutex = 0;
}

