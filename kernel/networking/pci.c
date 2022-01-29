#include "pci.h"

uint16_t read_dword_from_pci(uint8_t bus, uint8_t device, uint8_t func, uint8_t register_ofset);

uint16_t read_dword_from_pci(uint8_t bus, uint8_t device, uint8_t func, uint8_t register_ofset)
{
    uint32_t data_address = (((uint32_t)bus) << 16 |    // aligning bus 
    ((uint32_t)device << 11) |                         // aligning device
    ((uint32_t)func) << 8 |                            // aliging func
    (register_ofset & 0xFC)                            // and func other than 2 first bits
    );
    
    outb()
}