#include "ip.h"

extern uint32_t g_self_ip;

void parse_ip(ip_packet* packet, int packet_length);


void parse_ip(ip_packet* packet, int packet_length)
{
    // checking for ipv4 if not drop
    if(packet->version != IPV4_VERSION)
    {
        free(packet);
        return;
    }

    // if packet is fragmated then drop
    if(packet->fragmantation_offset & 0x1fff)
    {
        free(packet);
        return;
    }
}