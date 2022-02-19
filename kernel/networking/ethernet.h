#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include "../memory/heap.h"
#include "../../libc/memory.h"
#include "ethernet_driver.h"

#define HEADER_TYPE_IP  0x0800
#define HEADER_TYPE_ARP 0x0806

typedef struct ethernet_header{
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uint16_t ethernet_type;
}ethernet_header;

typedef struct ethernet_packet
{
    ethernet_header header;
    uint8_t content[];
}ethernet_packet;


/*
    This function parses the ethernet packet
    if arp packet sends to arp parse
    if ip sends to ip parse
    @param packet: the ethernet packet
    @param packet_len: the packet len
*/
void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len);

/*
    This function sends an ethernet packet
    @param packet: the packet content
    @param packet len: the packet len
    @param ethernet_type: the packet type (arp or ip)
    @param dest_mac: the destination mac address 
*/
void send_ethernet_packet(uint8_t* packet, uint32_t packet_len, uint16_t ethernet_type, uint8_t dest_mac[6]);
#endif