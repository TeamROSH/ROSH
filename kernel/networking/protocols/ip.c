#include "ip.h"

extern uint32_t g_self_ip;

void parse_ip(ip_packet* packet, int packet_length);
uint16_t calculate_ip_checksum(ip_packet* packet);
void send_ip_packet(void* packet_content, uint32_t packet_length, uint32_t destination_ip, uint8_t protocol);


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
    uint32_t checksum = 0;

    // increasing the checksum  with each byte of the ip header
    for(uint32_t i = 0; i + 1 < packet_header_length; i += 2)
    {
        checksum += *(uint8_t*)packet;
        packet += 1;
    }
    
    // if ip header size is an odd number 
    if(packet_header_length %  2 == 1)
    {
        // adding just last byte value
        checksum += *(uint8_t*)packet & 0xFF00;
    }

    // shifting the result into 16 bits
    while (checksum >> 16) 
    {
        checksum =  (checksum >> 16) + (checksum & 0xffff);
    }

    return (uint16_t)~checksum;
}

void send_ip_packet(void* packet_content, uint32_t packet_length, uint32_t destination_ip, uint8_t protocol)
{
    // the destination mac address
    uint8_t* dest_mac = NULL;

    // allocating spcae for the packet header and content
    ip_packet* packet = (ip_packet*)kmalloc(sizeof(ip_packet) + packet_length);

    // setting correct ip protocol version
    packet->version = IPV4_VERSION;

    // setting the ip header size 
    packet->ihl =IPV4_IHL;

    // not relavent to our protocol type
    packet->dscp = 0;
    packet->ecn = 0;

    // setting the ip packet + content
    packet->total_length = sizeof(ip_packet) + packet_length;

    // not relavent
    packet->identification = 0;

    // our os don't support fragmantation
    packet->flags = 0;
    packet->fragmantation_offset = 0;

    // setting packet time to live
    packet->ttl = IPV4_TTL;
    
    // setting source and destination
    packet->src_ip = g_self_ip;
    packet->dst_ip = destination_ip;

    // copying the packet content
    memcpy(packet + packet->ihl * 4, packet_content, packet_length);

    // calculating the packet checksum
    packet->checksum = calculate_ip_checksum(packet);

    // searching destnation mac address
    for(int i = 0; i < 5; i++)
    {   
        // looking for device in arp cache
        dest_mac = find_mac_via_ip(destination_ip);
        
        //  if device found
        if(dest_mac != NULL)
        {
            break;
        }

        // sending arp request
        send_arp(destination_ip);

        // sleeping and waiting for result
        sleep(2000);
    }

    // device mac address wasn't found
    if(dest_mac == NULL)
    {
        kfree(packet);
        kfree(packet_content);
        return;
    }

    // sending the packet
    send_ethernet_packet((uint8_t*)packet, packet->total_length, HEADER_TYPE_IP, dest_mac);
}