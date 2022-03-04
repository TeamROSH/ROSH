#ifndef UDP_H
#define UDP_H

#include <stdint.h>
#include "../../memory/heap.h"
#include "../../../libc/memory.h"
#include "../../../libc/string.h"
#include "../arp.h"
#include "ip.h"

#define DHCP_CLIENT_PORT 68 


typedef struct udp_packet{
    uint16_t source_port;       // packet sender port num
    uint16_t destination_port;  // packet reciver port num
    uint16_t length;            // udp header + content length
    uint16_t checksum;          // udp packet checksum (optiona in IPV4 so remain 0)
}udp_packet;

/*
    This function parses udp packet
    @param packet: the usp packet
*/
void parse_udp(udp_packet* packet);

/*
    This function sends a udp packet
    @param source_port: the sender port num
    @param destination_port: the destination port num
    @param content_length: the udp packet content length
    @param packet_content: the udp packet content
    @param destination_ip: the destination ip address
*/
void send_udp(uint16_t source_port, uint16_t destination_port, uint32_t content_length, void* packet_content, uint32_t destination_ip);

#endif