#include "ip.h"
#include "arp.h"
#include "ethernet.h"
#include "udp.h"

extern uint32_t g_self_ip;

void parse_ip(ip_packet* packet, int packet_length);
uint16_t calculate_ip_checksum(ip_packet* packet);
void send_ip_packet(void* packet_content, uint32_t packet_length, uint32_t destination_ip, uint8_t protocol);


void parse_ip(ip_packet* packet, int packet_length)
{
    // if packet is corrupted
    if(calculate_ip_checksum(packet) != 0)
        return;

	// get correct values
	uint8_t temp = 0;
	temp = packet->version;
	packet->version = packet->ihl;
	packet->ihl = temp;
	packet->total_length = (uint16_t)num_format_endian(&(packet->total_length), 2);

	// checking for ipv4 if not drop
    if(packet->version != IPV4_VERSION)
        return;

    // if packet is fragmated then drop
    if(packet->fragmantation_offset & 0x1fff)
        return;

    // calculating the packet length
    int parsed_packet_length = packet->total_length - sizeof(ip_packet);

    // if udp packet
    if(packet->protocol == IPV4_UDP_TYPE)
        parse_udp(packet);
}

uint16_t calculate_ip_checksum(ip_packet* packet)
{
    // the ihl contains the number of 32 bits of header size, converting it to number of bytes
    uint32_t packet_header_length = 10;
    uint32_t checksum = 0;
	uint8_t* ptr = (uint8_t*)packet;

    // increasing the checksum  with each byte of the ip header
    for(uint32_t i = 0; i < packet_header_length; i++)
    {
        checksum += *((uint16_t*)ptr);
        ptr += 2;
    }

    // shifting the result into 16 bits
    while (checksum >> 16) 
    {
        checksum = (checksum >> 16) + (checksum & 0xffff);
    }

    return (uint16_t)~checksum;
}

void send_ip_packet(void* packet_content, uint32_t packet_length, uint32_t destination_ip, uint8_t protocol)
{
    // the destination mac address
    uint8_t* dest_mac = NULL;
	uint8_t malloc_flag = 0;

    // allocating spcae for the packet header and content
    ip_packet* packet = (ip_packet*)kmalloc(sizeof(ip_packet) + packet_length);
	memset(packet, 0, sizeof(ip_packet));

    // setting correct ip protocol version
    packet->version = IPV4_VERSION;
	packet->protocol = protocol;

    // setting the ip header size 
    packet->ihl = IPV4_IHL;

    // not relavent to our protocol type
    packet->dscp = 0;
    packet->ecn = 0;

    // setting the ip packet + content
    packet->total_length = sizeof(ip_packet) + packet_length;
	// switch endian
	uint8_t temp = 0;
	temp = packet->version;
	packet->version = packet->ihl;
	packet->ihl = temp;
	packet->total_length = (uint16_t)num_format_endian(&(packet->total_length), 2); 

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
    memcpy(packet + 1, packet_content, packet_length);

	// calculating the packet checksum
    packet->checksum = calculate_ip_checksum(packet);

    if(packet->dst_ip != BROADCAST_IP)
    {
        // searching destnation mac address (5 times to 1s delay max)
        for(int i = 0; i < 5; i++)
		// while(1)
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
    }
    else
    {
        // if dhcp request mac address is broadcast
		malloc_flag = 1;
        dest_mac = (uint8_t*)kmalloc(6);
        dest_mac[0] = 255;
        dest_mac[1] = 255;
        dest_mac[2] = 255;
        dest_mac[3] = 255;
        dest_mac[4] = 255;
        dest_mac[5] = 255;
    }
    // sending the packet
    send_ethernet_packet((uint8_t*)packet, (uint16_t)num_format_endian(&(packet->total_length), 2), HEADER_TYPE_IP, dest_mac);
	
	if (malloc_flag)		// if allocated heap space
		kfree(dest_mac);

	kfree(packet);
}