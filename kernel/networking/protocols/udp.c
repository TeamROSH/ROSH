#include "udp.h"

void parse_udp(udp_packet* packet);
void send_udp(uint16_t source_port, uint16_t destination_port, uint32_t content_length, void* packet_content, uint32_t destination_ip);


void parse_udp(udp_packet* packet)
{
    // if recived packet is dhcp
    if(packet->destination_port == DHCP_CLIENT_PORT)
    {
        //parse_dhcp(packet + sizeof(udp_packet));
    }
}

void send_udp(uint16_t source_port, uint16_t destination_port, uint32_t content_length, void* packet_content, uint32_t destination_ip)
{
    // allocating space for new packet
    udp_packet* packet = (udp_packet*)kmalloc(sizeof(udp_packet) + content_length);

    // assigning source and dest port
    packet->source_port = source_port;
    packet->destination_port = destination_port;

    // setting packet length
    packet->length = content_length + sizeof(udp_packet);

    // ipv4 doesn't require udp checksum
    packet->checksum = 0; 

    // copying content to the new packet
    memcpy(packet + sizeof(udp_packet), packet_content, content_length);
    
    // sending the udp packet
    send_ip_packet((void*)packet, sizeof(udp_packet) + content_length, destination_ip, IPV4_UDP_TYPE);
}