#include "elf.h"
/*
extern page_directory* g_page_directory;

bool validate_elf(elf_header* elf);
bool load_process_from_elf(char* elf_path, process_context_block* pcb);


bool validate_elf(elf_header* elf)
{
    if(strncmp(elf->e_ident, ELF_MAGIC_NUM ,4) == 0)
    {
        return true; 
    }

    return false;
}

bool load_process_from_elf(char* elf_path, process_context_block* pcb)
{
    elf_header* elf = (elf_header*)kmalloc(sizeof(elf_header));
    char* res = NULL;
    uint32_t file_base = 0xffffffff;
    program_header* elf_program_header = NULL;
    int program_size = 0;

    
    TODO 
    remove form comment when will be merged into fs branch
    remember to tell Roei to kmalloc res because fs has the file size
    TODO 
    if(read_file(elf_path, res) == NULL)
    {
        return false;
    }
    

   // coping elf header 
    memcpy(&elf, res, sizeof(elf_header)); 

   // if not elf
    if(validate_elf(elf) == false)
    {
       return false; 
    }


    // calculating program header base 
    for(int i = 0; i < elf->e_phnum; i++)
    {
        elf_program_header = res + 
        elf->e_phoff + (elf->e_phentsize * i;
        if(elf_program_header < base)
        {
            base = elf_program_header;
        }

    }

    // calculating program max size
    for(int i = 0; i < elf.e_phnum; i++)
    {
        elf_program_header = res + elf.e_phoff + elf.e_phentsize * i;
        if(elf_program_header->p_vaddr + elf_program_header->p_memsz  - file_base > program_size)
        {
            program_size = elf_program_header->p_vaddr + elf_program_header->p_memsz  - file_base;
        }

    }

    int j = 0, k =0;
    for(int i = 0; i < elf.e_phnum; i++)
    {
        elf_program_header = res + elf.e_phoff + elf.e_phentsize * i;
        int num_of_pages = elf_program_header->p_memsz/ 4096 + 1; 
        while( j = 0; j < num_of_pages; j ++)
        {
            pcb->process_pages[k + 5] = page_to_address(page_alloc());
            page_map(&g_page_directory, pcb->process_pages[k + 5], pcb->process_pages[k + 5], 
            flags & PAGE_FLAG_READONLY) ? 1 : 0, 
            (flags & PAGE_FLAG_USER) ? 0 : 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0););
            k++;
        }

    }

}
*/