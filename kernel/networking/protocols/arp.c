#include "arp.h"
#include "ethernet.h"

device_address g_address_cache[ARP_CACHE_LEN] = {0};

extern uint8_t g_src_mac[6];
extern uint32_t g_self_ip;
uint32_t g_arp_waiting = 0;
uint32_t g_time_saver = 0;

void parse_arp_packet(arp_packet* packet, uint32_t packet_len);
int find_arp_device(device_address* device);
uint8_t* find_mac_via_ip(uint32_t ip_address);
void create_and_send_arp(uint32_t src_ip, uint32_t dest_ip, uint8_t src_mac[6], uint8_t dst_mac[6], uint16_t opcode);
void send_arp(uint32_t dst_ip);

void parse_arp_packet(arp_packet* packet, uint32_t packet_len)
{
    //  assigning the computers addresses
    uint32_t src_ip = packet->srcpr;
    uint32_t dst_ip = packet->dstpr;
    uint8_t src_mac[6] = {0};
    uint8_t dst_mac[6] = {0};

    // setting mac addresses
    memcpy(src_mac, packet->srchw, 6);
    memcpy(dst_mac, packet->dsthw, 6);

    if(packet->opcode == OPERATION_ARP_REQUEST)
    {
        device_address* request_device = (device_address*)kmalloc(sizeof(device_address));
		memset(request_device, 0, sizeof(device_address));
        for(int i = 0; i < ARP_CACHE_LEN; i++)
        {   
            // if requested device found
            if(g_address_cache[i].ip_address == dst_ip)
            {
                request_device->ip_address = dst_ip;
                memcpy(request_device->mac_address, g_address_cache[i].mac_address, 6);
                break;
            }
        } 

        // sending the result if found device in the arp cache
        if(request_device->ip_address != NULL)
        {
            create_and_send_arp(dst_ip, src_ip, request_device->mac_address, src_mac, OPERATION_ARP_REPLAY);
        }
		kfree(request_device);
    }

    // if arp replay to our previous arp request 
    else if(packet->opcode == OPERATION_ARP_REPLAY)
    {   
        // going through the arp cache
        for(int i = 0; i < ARP_CACHE_LEN; i++)
        {  
            // if saving the device address
            if(g_address_cache[i].ip_address == 0)
            {
                g_address_cache[i].ip_address = packet->srcpr;
                memcpy(g_address_cache[i].mac_address, packet->srchw, 6);
                break;
            }
        }
		if (g_arp_waiting != 0 && g_arp_waiting == packet->srcpr)		// print reply
		{
			g_arp_waiting = 0;

			if (get_time() - g_time_saver < 2500)
			{
				puti(((uint8_t*)&(packet->srcpr))[0]);
				for (int i = 1; i < 4; i++)
				{
					putc('.'); puti(((uint8_t*)&(packet->srcpr))[i]);
				}
				puts(" is at ");
				putc(hex(packet->srchw[0] / 16)); putc(hex(packet->srchw[0] % 16));
				for (int i = 1; i < 6; i++)
				{
					putc(':'); putc(hex(packet->srchw[i] / 16)); putc(hex(packet->srchw[i] % 16));
				}
			}
		}
    }
}

int find_arp_device(device_address* device)
{
    for(int i = 0; i < ARP_CACHE_LEN; i++)
    {   
        // if device found
        if(strncmp((char*)&(g_address_cache[i]), (char*)device, sizeof(device_address)))
        {
            return i;
        }
    }

    // device wasn't found
    return -1;
}

uint8_t* find_mac_via_ip(uint32_t ip_address)
{
    for(int i = 0; i < ARP_CACHE_LEN; i++)
    {   
        // if device found via ip address
        if(g_address_cache[i].ip_address == ip_address && ip_address != NULL)
        {
            return g_address_cache[i].mac_address;
        }
    }

    // device wasn't found
    return NULL;
}

void create_and_send_arp(uint32_t src_ip, uint32_t dest_ip, uint8_t src_mac[6], uint8_t dst_mac[6], uint16_t opcode)
{
    arp_packet* packet = (arp_packet*)kmalloc(sizeof(arp_packet));

    // setting ethernet and ip
    packet->hlen = ETHERNET_HLEN;
    packet->plen = IPV4_PLEN;

    // setting ethernet hardware type
    packet->htype = 0x0100;

    // setting ipv4 protocol
    packet->ptype = 0x0008;

    // setting arp opcode
    packet->opcode = opcode;

    // setting mac addresses
    memcpy(packet->srchw, src_mac, 6);
    memcpy(packet->dsthw, dst_mac, 6); 

    // setting ip addresses
    packet->srcpr = src_ip;
    packet->dstpr = dest_ip;

    // sending arp packet
    send_ethernet_packet((uint8_t*)packet, sizeof(arp_packet), HEADER_TYPE_ARP, dst_mac);
}

void send_arp(uint32_t dst_ip)
{
    // brodcast dst mac
    uint8_t dst_mac[6] = {255, 255, 255, 255, 255, 255};
    
    // sending the arp requst
    create_and_send_arp(g_self_ip, dst_ip, g_src_mac, dst_mac, OPERATION_ARP_REQUEST);
}

void send_arp_user(uint32_t dst_ip)
{
	send_arp(dst_ip);
	g_arp_waiting = dst_ip;
	g_time_saver = get_time();
}