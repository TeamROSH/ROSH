#ifndef ELF_H
#define ELF_H
#include <stdint.h>
#include "../../libc/string.h"
#include "../process/process.h"

#define ELF_MAGIC_NUM "\177ELF"

typedef struct elf_header{
  uint8_t e_ident[4]; 
  uint16_t e_type;
  uint16_t e_machine;
  uint32_t e_version;
  uint32_t e_entry;
  uint32_t e_phoff; 
  uint32_t e_shoff;
  uint32_t e_flags;
  uint16_t e_ehsize;
  uint16_t e_phentsize; 
  uint16_t e_phnum; 
  uint16_t e_shentsize;
  uint16_t e_shnum;
  uint16_t e_shstrndx;
} elf_header;
 
typedef struct program_header {
  uint32_t p_type;
  uint32_t p_offset; 
  uint32_t p_vaddr; 
  uint32_t p_paddr;
  uint32_t p_filesz; 
  uint32_t p_memsz;
  uint32_t p_flags;
  uint32_t p_align; 
} program_header;



/*
    validates elf signature
    @param elf: pointer to the elf header
    retruns true if valid and false if not
*/
bool validate_elf(elf_header* elf);

/*
  this function loads the elf file into process
  @param process_path: pointer to the file path
  @param pcb: pointer to the process context block
*/
bool load_process_from_elf(char* elf_path, process_context_block* pcb);


#endif