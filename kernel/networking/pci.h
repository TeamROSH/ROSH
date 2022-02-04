#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include "../ports/ports.h"
#include "../memory/heap.h"

// physical address of io ports of pci
#define CONFIG_ADDRESS 0XCF8
#define CONFIG_DATA 0XCFC

// number of buses and devices in pci
#define BUS_NUM 256
#define DEVICE_NUM 32 

#define RTL8139_VENDOR_ID 0x10EC 
#define RTL8139_DEVICE_ID 0x8139

//#define ETHERNET_SUBCLASS 0
//#define ETHERNET_CLASS_CODE 2

// pci devices header types
#define HEADER_DEFUALT 0x0
#define HEADER_PCI_TO_PCI 0X1
#define HEADER_PCI_TO_CARDBUS 0X2
#define HEADER_MULTY_FUNCTION 0x80

// num of bars in header defualt pci device
#define BAR_NUM 5

#ifndef NULL
#define NULL 0
#endif

typedef struct bar{
    union 
    {
        uint32_t address;
        uint32_t io_address;
    }value;

    // 1 - I/O
    // 0 - memory address
    uint32_t type;
    
}bar;

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
    bar bars[5];
}pci_header_data;

typedef struct device_data{
    uint8_t bus_num;
    uint8_t device_num;
    pci_header_data* device_header;
}device_data;

/*
    This function reads a 16 bit value from the pci register
    @param bus: the pci bus number
    @param device: the pci device number
    @param func: the pci device function
    @param regiter_offset: the pci register offset 
    This function returns the data requested
*/
uint16_t read_word_from_pci(uint8_t bus, uint8_t device, uint8_t func, uint8_t register_ofset);

/*
    This function reads a 32 bit value from the pci register
    @param bus: the pci bus number
    @param device: the pci device number
    @param func: the pci device function
    @param regiter_offset: the pci register offset 
    This function returns the data requested
*/
uint32_t read_dword_from_pci(uint8_t bus, uint8_t device, uint8_t func, uint8_t register_ofset);


/*
    This function gets pci header data of pci device
    @param bus: the pci bus number
    @param device: the pci device number
    returns the pci device header data
*/
pci_header_data* get_pci_device_data(uint8_t bus, uint8_t device);


/*
    This function gets pci device data
    @param device_id: the pci device id
    @param vendor_id: the pci device vendor id
    This function returns the pci device data
    if not found returns null
*/
device_data* get_pci_device(uint16_t device_id, uint16_t vendor_id);

/*
    This functon gets the ethernet controller device data
    if not found returns 0
*/
device_data* get_ethernet_controller();

/*
    This function reads the bar information from type 0 pci device
    @parma header: the pci device header struct
    @param bus: the pci bus number
    @param device: the pci device number
*/
void read_device_bars(pci_header_data* header, uint8_t bus, uint8_t device);

#endif