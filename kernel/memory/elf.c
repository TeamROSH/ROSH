#include "elf.h"

bool validate_elf(elf_header* elf);


bool validate_elf(elf_header* elf)
{
    if(strncmp(elf->e_ident, ELF_MAGIC_NUM ,4) == 0)
    {
        return true; 
    }

    return false;
}