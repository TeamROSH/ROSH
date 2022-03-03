#include "ip.h"

extern uint32_t g_self_ip;

void parse_ip(ip_packet* packet, int packet_length);
uint16_t calculate_ip_checksum(ip_packet* packet);


void parse_ip(ip_packet* packet, int packet_length)
{
    // checking for ipv4 if not drop
    if(packet->version != IPV4_VERSION)
    {
        kfree(packet);
        return;
    }

    // if packet is fragmated then drop
    if(packet->fragmantation_offset & 0x1fff)
    {
        kfree(packet);
        return;
    }
    
    // if packet is corrupted
    if(calculate_ip_checksum(packet) != packet->checksum)
    {
        kfree(packet);
        return;
    }

    // calculating the packet length
    int parsed_packet_length = packet->total_length - sizeof(ip_packet);

    if(packet->protocol == IPV4_TCP_TYPE)
    {
        // parse_tcp((void*)packet + packet->ihl * 4, parsed_packet_length);
    }

    // if udp packet
    else if(packet->protocol == IPV4_UDP_TYPE)
    {
        // parse_udp((void*)packet + packet->ihl * 4, parsed_packet_length);
    }

    // if icmp packet
    else if(packet->protocol == IPV4_ICMP_TYPE)
    {
        // parse_icmp((void*)packet + packet->ihl * 4, parsed_packet_length);
    }
}

uint16_t calculate_ip_checksum(ip_packet* packet)
{
    // the ihl contains the number of 32 bits of header size, converting it to number of bytes
    uint32_t packet_header_length = packet->ihl << 2;
    uiint32_t checksum = 0;

    // increasing the checksum  with each byte of the ip header
    for(uint32_t i = 0; i + 1 < packet_header_length; i += 2)
    {
        checksum += *packet
        packet += 1;
    }
    
    // if ip header size is an odd number 
    if(packet_header_length %  2 == 1)
    {
        // adding just last byte value
        checksum += *packet &0xFF00
    }

    // shifting the result into 16 bits
    while (checksum >> 16) 
    {
        checksum =  (checksum >> 16) + (checksum & 0xffff);
    }

    return (uint16_t)~checksum;
}