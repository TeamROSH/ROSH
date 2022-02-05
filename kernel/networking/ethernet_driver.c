#include "ethernet_driver.h"

ethernet_device* g_ethernet_device;

void initialize_ethernet_driver();
void network_handler(registers_t* registers);
void read_mac_address();


void initialize_ethernet_driver()
{
    g_ethernet_device = (ethernet_device*)kmalloc(sizeof(ethernet_device));
    
    // getting ethernet device data
    g_ethernet_device->ethernet_device_data = get_ethernet_controller();


    // setting ethernet device io address
    g_ethernet_device->io_base = g_ethernet_device->ethernet_device_data->device_header->bars[0].value.io_address;

    uint32_t io_base = g_ethernet_device->io_base;

    // turning on the ethernet device
    outb(io_base + IO_CONFIG1_OFFSET, 0 );

    // reseting ethernet device
    outb(io_base + IO_CMD_OFFSET, 0x10);    

    // waiting for reseting to finish
    while(inb(io_base + IO_CMD_OFFSET) & 0x10 !=0){}


    //TODO 
    // make sure that setting the virt and not phys address is ok
    //TODO
    // allocating packet input buffer 
    uint32_t virtual_input_buffer = (uint32_t)kmalloc(RX_BUFFER_LEN);
    
    // setting buffer for input packets
    outdw(io_base + IO_RBSTART_OFFSET, (uint32_t)virtual_input_buffer);

    // setting flags for interrupts
    outw(io_base + IO_IMR_OFFSET, 0x0005);

    // configure recive buffer
    outw(io_base + IO_RCR_CONFIGURE, 0x8F);

    // setting recive and transmits
    outb(io_base + IO_CMD_OFFSET, 0x0C);

    // setting interrupt handler
    set_interrupt(g_ethernet_device->ethernet_device_data->device_header->interrupt_line + IRQ0, network_handler);

    // reading the mac address
    read_mac_address();
}

void network_handler(registers_t* registers)
{
    
}

void read_mac_address()
{
    // reading the mac address
    g_ethernet_device->mac_address[0] = inb(g_ethernet_device->io_base); 
    g_ethernet_device->mac_address[1] = inb(g_ethernet_device->io_base + 1); 
    g_ethernet_device->mac_address[2] = inb(g_ethernet_device->io_base + 2); 
    g_ethernet_device->mac_address[3] = inb(g_ethernet_device->io_base + 3); 
    g_ethernet_device->mac_address[4] = inb(g_ethernet_device->io_base + 4); 
    g_ethernet_device->mac_address[5] = inb(g_ethernet_device->io_base + 5);   
}