#include "udp.h"
#include "arp.h"
#include "ip.h"
#include "dhcp.h"
#include "../../../libc/screen.h"

void parse_udp(void* packet);
void send_udp(uint16_t source_port, uint16_t destination_port, uint32_t content_length, void* packet_content, uint32_t destination_ip);


void parse_udp(void* packet)
{
	ip_packet* ip = (ip_packet*)packet;
	udp_packet* udp = (udp_packet*)(ip + 1);
	udp->length = (uint16_t)num_format_endian(&(udp->length), 2);
	udp->destination_port = (uint16_t)num_format_endian(&(udp->destination_port), 2);
	udp->source_port = (uint16_t)num_format_endian(&(udp->source_port), 2);
    // if recived packet is dhcp
    if(udp->destination_port == DHCP_CLIENT_PORT)
    {
        parse_dhcp((dhcp_packet*)(udp + 1));
    }

	if(udp->destination_port == ROSH_PORT)
    {
        char* msg = (char*)(udp + 1);
		putc('\n');
		puti(((uint8_t*)&(ip->src_ip))[0]);
		for (int i = 1; i < 4; i++)
		{
			putc('.'); puti(((uint8_t*)&(ip->src_ip))[i]);
		}
		puts(": "); puts(msg); putc('\n');
    }
}

void send_udp(uint16_t source_port, uint16_t destination_port, uint32_t content_length, void* packet_content, uint32_t destination_ip)
{
    // allocating space for new packet
    udp_packet* packet = (udp_packet*)kmalloc(sizeof(udp_packet) + content_length);

    // assigning source and dest port
    packet->source_port = source_port;
    packet->destination_port = destination_port;

	packet->destination_port = (uint16_t)num_format_endian(&(packet->destination_port), 2);
	packet->source_port = (uint16_t)num_format_endian(&(packet->source_port), 2);

    // setting packet length
    packet->length = content_length + sizeof(udp_packet);
    packet->length = (uint16_t)num_format_endian(&(packet->length), 2);

    // ipv4 doesn't require udp checksum
    packet->checksum = 0; 

    // copying content to the new packet
    memcpy(packet + 1, packet_content, content_length);
    
    // sending the udp packet
    send_ip_packet((void*)packet, sizeof(udp_packet) + content_length, destination_ip, IPV4_UDP_TYPE);
}