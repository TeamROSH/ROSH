#include "ethernet_driver.h"

ethernet_device* g_ethernet_device;
uint8_t g_ts_reg[] = {0x20, 0x24, 0x28, 0x2C};
uint8_t g_tc_reg[] = {0x10, 0x14, 0x18, 0x1C};
void initialize_ethernet_driver();
void network_handler(registers_t* registers);
void send_packet(void* content, uint32_t packet_len);
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

    g_ethernet_device->curr_reg = 0;

    // setting interrupt handler
    set_interrupt(g_ethernet_device->ethernet_device_data->device_header->interrupt_line + IRQ0, network_handler);

    // reading the mac address
    read_mac_address();
}

void network_handler(registers_t* registers)
{
    uint16_t isr_value= inw(g_ethernet_device->io_base + IO_ISR_OFFSET);

    // if packet recived (ROK bit set)
    if(isr_value & 1)
    {
        
    }
}

void send_packet(void* content, uint32_t packet_len)
{
    uint32_t transmit_buff = (uint32_t)kmalloc(packet_len);

    // making sure and allocating space for packet in kernel heap
    memcpy((void*)transmit_buff, content, packet_len);

    // sending packet data
    outdw((uint16_t)g_ethernet_device->io_base + g_ts_reg[g_ethernet_device->curr_reg], transmit_buff);
    
    // sending packet len
    outdw((uint16_t)g_ethernet_device->io_base + g_tc_reg[g_ethernet_device->curr_reg], packet_len);
    
    // only 4 elements in array
    g_ethernet_device->curr_reg = (++(g_ethernet_device->curr_reg)) > 3 ? 0 : g_ethernet_device->curr_reg; 
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