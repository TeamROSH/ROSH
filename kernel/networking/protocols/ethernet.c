#include "ethernet.h"
#include "arp.h"
#include "ip.h"

void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len);
void send_ethernet_packet(uint8_t* packet, uint32_t packet_len, uint16_t ethernet_type, uint8_t dest_mac[6]);

extern uint8_t g_src_mac[6];

void parse_ethernet_packet(ethernet_packet* packet, uint32_t packet_len)
{
	uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	// if not our mac or broadcast
	if (strncmp(packet->header.destination_address, g_src_mac, 6) != 0 && strncmp(packet->header.destination_address, broadcast, 6) != 0)
		return;
    // if arp packet 
    if(packet->header.ethernet_type == HEADER_TYPE_ARP)
    {
        parse_arp_packet((arp_packet*)((uint32_t)packet + sizeof(ethernet_header)), packet_len - sizeof(ethernet_header));
    }
    
    // if ip packet
    else if(packet->header.ethernet_type == HEADER_TYPE_IP)
    {
        parse_ip((ip_packet*)((uint32_t)packet + sizeof(ethernet_header)), packet_len - sizeof(ethernet_header));
    }
}

void send_ethernet_packet(uint8_t* packet, uint32_t packet_len, uint16_t ethernet_type, uint8_t* dest_mac)
{
    // allocating sent packet len
    uint8_t* sent_packet = kmalloc(sizeof(ethernet_header) + packet_len);
    
    // setting packet type
    ((ethernet_packet*)sent_packet)->header.ethernet_type = ethernet_type;
    
    // setting packet mac addresses
    memcpy(((ethernet_packet*)sent_packet)->header.source_address, g_src_mac, 6);
    memcpy(((ethernet_packet*)sent_packet)->header.destination_address, dest_mac, 6);
    
    // setting packet content
    memcpy(sent_packet + sizeof(ethernet_header), packet, packet_len);

    // sending the packet
    send_packet(sent_packet, packet_len + sizeof(ethernet_header));
    kfree(sent_packet);
}