#include "process.h"

extern page_directory* g_page_directory;

int g_highest_pid = 1;
process_context_block* g_curr_process; 

list* g_process_list;
list* g_ready_processes_list;
int times = 0;

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

extern void context_jump(uint32_t es, uint32_t cs, uint32_t esp, uint32_t eip, registers_t registers);
extern void usermode(void);


process_context_block* init_usermain_process()
{
	process_context_block* pcb = (process_context_block*)kmalloc(sizeof(process_context_block));
	pcb->is_kernel = 0;
    pcb->pid = generate_pid();
	memcpy(pcb->name, "usermain", PROCESS_NAME);
	initialize_process_regs(pcb);

	pcb->process_pages[0] = USER_STACK_START_ADDR;
	pcb->stack_base = pcb->process_pages[0];

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

    //alloc process page directory
    // pcb->curr_page_directory = (page_directory*)kmalloc(sizeof(page_directory));
    // memcpy(pcb->curr_page_directory, g_page_directory, sizeof(page_directory));

    //mapping stack
    pcb->process_pages[0] = page_to_address(page_alloc());
    pcb->process_pages[1] = page_to_address(page_alloc());
    pcb->stack_base = pcb->process_pages[0];
	pcb->reg.esp = pcb->process_pages[1] + PAGE_SIZE - 4;

    // allocating apace for heap
    for(int i =2; i < 5; i ++)
    {
        pcb->process_pages[i] = page_to_address(page_alloc());
    }
    
    // crating heap object 
    heap_init(&(pcb->process_heap), pcb->process_pages[2], PAGE_SIZE * 3);

    load_process_code(pcb, process_name);

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
		pcb->process_pages[i + 5] = page_to_address(page_alloc());
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
        page_free(address_to_page(pcb->process_pages[i]));
        memset((void*)page_to_address(pcb->process_pages[i]), 0, PAGE_SIZE);
    }
	delete_node_by_data(g_process_list, pcb);
	delete_node_by_data(g_ready_processes_list, pcb);

    kfree(pcb);
}

void process_scheduler(registers_t* registers)
{
	if (times > 1)
		return;
    node* next_process;
    for (next_process = g_ready_processes_list->head; next_process != NULL; next_process = next_process->next)
	{
		process_context_block* proc = ((process_context_block*)next_process->data);
		if (proc->process_state == PROCESS_READY)
		{
			// saving the old process registers
			save_registers(g_curr_process, registers);

			// pushing the current process to the list head
			insert_tail(g_ready_processes_list, g_curr_process);
			g_curr_process->process_state = PROCESS_READY;

			delete_node_by_data(g_ready_processes_list, proc);

			times++;

			// context switching to the next process

			asm volatile("sti");
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
    next_process->process_state = PROCESS_RUNNING;
	g_curr_process = next_process;
    // for (int i = 0; i < MAX_PROCESS_PAGES; i++)
    // {
    //     // unmaping the old process meory
    //     page_unmap(g_curr_process->process_pages[i]);

    //     // mapping the new process memory
    //     page_map(next_process->curr_page_directory, next_process->process_pages[i], next_process->process_pages[i], PAGE_FLAG_READWRITE | PAGE_FLAG_USER);
    // }
	context_jump(next_process->reg.es, next_process->reg.cs, next_process->reg.esp, next_process->reg.eip, next_process->reg);
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