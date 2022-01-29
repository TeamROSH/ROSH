#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include "../ports/ports.h"

// physical address of io ports of pci
#define CONFIG_ADDRESS 0XCF8
#define CONFIG_DATA 0XCFC

/*
    This function reads a 16 bit value from the pci register
    @param bus: the pci bus number
    @param device: the pci device number
    @param func: the pci device function
    @param regiter_offset: the pci register offset 
*/
uint16_t read_dword_from_pci(uint8_t bus, uint8_t device, uint8_t func, uint8_t register_ofset);

#endif