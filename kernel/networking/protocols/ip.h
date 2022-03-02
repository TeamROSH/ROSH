#ifndef IP_H
#define IP_H

#include <stdint.h>
#include "../memory/heap.h"
#include "../../libc/memory.h"
#include "../../libc/string.h"
#include "../arp.h"
#define IPV4_VERSION 4

// ipv4 protocols types
#define IPV4_UDP_TYPE 0X11
#define IPV4_TCP_TYPE 0x6
#define IPV4_ICMP_TYPE 0x1

typedef struct ip_packet{
    uint8_t version : 4; // ip packet version (ipv4=4)
    uint8_t ihl : 4;     // packet length multipllied with 32 bits
    uint8_t dscp : 6;    // not relevant used for voip
    uint8_t ecn : 2;     // network congestion without droping packets
    uint16_t total_length;  // packet and header length
    uint16_t identification;
    uint8_t flags : 3;      // Don't fradment and More Fragments bits 
    uint16_t fragmantation_offset : 13; // fragmantation isn't supported 
    uint8_t ttl;            // packets time to live 
    uint8_t protocol;       // packets protocol type 
    uint16_t checksum;      // packets checksum (different between tcp and udp)
    uint32_t src_ip;        // packets source ip
    uint32_t dst_ip;        // packets destination ip 
}ip_packet;


/*
    This funtion parses an ip packet and sends the parsed packet to the wanted destination
    @param packet: the ip packet
    @param packet_length: the packet length
*/
void parse_ip(ip_packet* packet, int packet_length);


#endif