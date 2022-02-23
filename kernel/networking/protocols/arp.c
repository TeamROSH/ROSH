#include "arp.h"

device_address g_address_cache[ARP_CACHE_LEN];

void parse_arp_packet(arp_packet* packet, uint32_t packet_len);
int find_arp_device(device_address* device);

void parse_arp_packet(arp_packet* packet, uint32_t packet_len)
{
    //  assigning the computers addresses
    uint32_t src_ip = packet->srcpr;
    uint32_t dst_ip = packet->dstpr;
    uint8_t src_mac[6] = {0};
    uint8_t dst_mac[6] = {0};

    // setting mac addresses
    memcpy(src_mac, packet->srchw, sizeof(uint8_t[6]));
    memcpy(dst_mac, packet->dsthw, sizeof(uint8_t[6]));

    if(packet->opcode == OPERATION_ARP_REQUEST)
    {
        
    }

    // if arp replay to our previous arp request 
    else if(packet->opcode == OPERATION_ARP_REPLAY)
    {   
        // going through the arp cache
        for(int i = 0; i < ARP_CACHE_LEN; i++)
        {  
            // if saving the device address
            if(strncmp(g_address_cache[i], 0, sizeof(device_address)) == 0)
            {
                g_address_cache[i].ip_address = packet->srcpr;
                memcpy(g_address_cache[i].mac_address, packet->srchw, sizeof(uint8_t[6]));
                break;
            }
        }
    }
}

int find_arp_device(device_address* device)
{
    for(int i = 0; i < ARP_CACHE_LEN; i++)
    {   
        // if device found
        if(strncmp(g_address_cache[i], *(device), sizeof(device_address)))
        {
            return i;
        }
    }

    // device wasn't found
    return -1;
}