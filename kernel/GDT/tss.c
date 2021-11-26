#include "tss.h"
#include "../../libc/memory.h"
#include "../memory/memorylayout.h"

tss_entry tss;

void init_tss(gdt_entry_bits* entry)
{
	uint32_t base = (uint32_t)&tss;		// get base address of tss
	uint32_t limit = sizeof(tss_entry);		// get size

	// init tss entry in the GDT
	entry->limit_low = limit;
	entry->base_low = base;
	entry->accessed = 1;		// TSS
	entry->read_write = 0;
	entry->conforming = 0;		// 0 for TSS
	entry->code = 1;			// 32-bit
	entry->code_data_segment=0;	// TSS
	entry->DPL = 0;				// ring 0
	entry->present = 1;
	entry->limit_high = (limit & (0xf << 16)) >> 16;
	entry->available = 0;		// TSS
	entry->long_mode = 0;
	entry->big = 0;
	entry->gran = 0;
	entry->base_high = (base & (0xff << 24)) >> 24;

	memset(&tss, 0, sizeof(tss_entry));		// zero tss

	tss.ss0 = 0x10;		// kernel data segment
	tss.esp0 = KERNEL_STACK_START_ADDR;		// kernel stack
}

void set_kernel_stack(uint32_t stack)
{
	tss.esp0 = stack;
}