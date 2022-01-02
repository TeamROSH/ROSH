#include "elf.h"

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
    elf_header elf = NULL;
    char* res = NULL;
    uint32_t file_base = 0xffffffff;
    program_header* elf_program_header = NULL;
    int program_size = 0;

    /*
    TODO 
    remove form comment when will be merged into fs branch
    remember to tell Roei to kmalloc res because fs has the file size
    TODO 
    if(read_file(elf_path, res) == NULL)
    {
        return false;
    }
    */

   // if not elf
    if(validate_elf(elf) == false)
    {
       return false; 
    }

    // coping elf header 
    memcpy(&elf, res, sizeof(elf_header));

    // calculating program header base 
    for(int i = 0; i < elf.e_phnum; i++)
    {
        elf_program_header = res + elf.e_phoff + elf.e_phentsize * i;
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

    uint32_t elf_add = kmalloc(sizeof(program size));
    for(int i = 0; i < elf.e_phnum; i++)
    {
        //memcpy()
    }

}