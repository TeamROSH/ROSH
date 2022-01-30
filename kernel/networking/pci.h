#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include "../ports/ports.h"
#include "../memory/heap.h"

// physical address of io ports of pci
#define CONFIG_ADDRESS 0XCF8
#define CONFIG_DATA 0XCFC

#ifndef NULL
#define NULL 0
#endif


typedef struct pci_header_data{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
}pci_header_data;
/*
    This function reads a 16 bit value from the pci register
    @param bus: the pci bus number
    @param device: the pci device number
    @param func: the pci device function
    @param regiter_offset: the pci register offset 
    This function returns the data requested
*/
uint16_t read_dword_from_pci(uint8_t bus, uint8_t device, uint8_t func, uint8_t register_ofset);

/*
    This function gets pci header data of pci device
    @param bus: the pci bus number
    @param device: the pci device number
    returns the pci device header data
*/
pci_header_data* get_pci_device_data(uint8_t bus, uint8_t device);

#endif