#ifndef ETHERNET_DRIVER_H
#define ETHERNET_DRIVER_H

#include "pci.h"
#include "idt.h"


// device registers offsets
#define IO_MAC_OFFSET 0
#define IO_MAR_OFFSET 0x08
#define IO_RBSTART_OFFSET 0X30
#define IO_CMD_OFFSET 0x37
#define IO_IMR_OFFSET 0X3C
#define IO_ISR_OFFSET 0x3E
#define IO_CONFIG1_OFFSET 0X52
#define IO_RCR_CONFIGURE 0X44

// input packet buffer
#define RX_BUFFER_LEN 9708


typedef struct ethernet_device{
    device_data* ethernet_device_data;
    uint32_t io_base;
    uint32_t rx_buff;
    uint32_t tx_buff;
    char[6] mac_address;
}ethernet_device;

/*
    This function intializes the RTL8139 ethernet card
*/
initialize_ethernet_driver();

/*
    This funciton handles recived packet interrupt
    @param registers: the curr regiter values
*/
void network_handler(registers_t* registers);

/*
    This function reads the mac address and updates the g_ethernet_device 
*/
void read_mac_address();
#endif