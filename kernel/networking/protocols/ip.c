#include "ip.h"

extern uint32_t g_self_ip;

void parse_ip(ip_packet* packet, int packet_length);


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