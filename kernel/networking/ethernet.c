#include "ethernet.h"

void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len);


void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len)
{
    // if arp packet 
    if(packet->header.ethernet_type == HEADER_TYPE_ARP)
    {
        // parse_arp_packet((arp_packet*)packet + sizeof(ethernet_packet), packet_len - sizeof(ethernet_packet))
    }
    
    // if ip packet
    else if(packet->header.ethernet_type == HEADER_TYPE_IP)
    {
        // parse_ip_packet((ip_packet*)packet + sizeof(ethernet_packet), packet_len - sizeof(ethernet_packet))
    }
}