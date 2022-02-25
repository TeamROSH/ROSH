#include "ethernet.h"

void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len);
void send_ethernet_packet(uint8_t* packet, uint32_t packet_len, uint16_t ethernet_type, uint8_t dest_mac[6]);

extern uint8_t g_src_mac[6];

void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len)
{
    // if arp packet 
    if(packet->header.ethernet_type == HEADER_TYPE_ARP)
    {
        // parse_arp_packet((arp_packet*)packet + sizeof(ethernet_packet), packet_len - sizeof(ethernet_packet));
    }
    
    // if ip packet
    else if(packet->header.ethernet_type == HEADER_TYPE_IP)
    {
        // parse_ip_packet((ip_packet*)packet + sizeof(ethernet_packet), packet_len - sizeof(ethernet_packet))
    }
}

void send_ethernet_packet(uint8_t* packet, uint32_t packet_len, uint16_t ethernet_type, uint8_t* dest_mac)
{
    // allocating sent packet len
    uint8_t* sent_packet = kmalloc(sizeof(ethernet_header) + packet_len);
    
    // setting packet type
    ((ethernet_packet*)sent_packet)->header.ethernet_type = ethernet_type;
    
    // setting packet mac addresses
    memcpy(((ethernet_packet*)sent_packet)->header.source_address, g_src_mac, 6);
    memcpy(((ethernet_packet*)sent_packet)->header.destination_address, dest_mac, 6);
    
    // setting packet content
    memcpy(sent_packet + sizeof(ethernet_header), packet, packet_len);

    // sending the packet
    send_packet(sent_packet, packet_len + sizeof(ethernet_header));
    kfree(sent_packet);
}