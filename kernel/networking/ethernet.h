#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

#define HEADER_TYPE_IP  0x0800
#define HEADER_TYPE_ARP 0x0806

typedef struct ethernet_header{
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uit16_t ethernet_type[2];
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


#endif