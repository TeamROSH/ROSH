#include "arp.h"

device_address g_address_cache[ARP_CACHE_LEN];

void parse_arp_packet(arp_packet* packet, uint32_t packet_len);

void parse_arp_packet(arp_packet* packet, uint32_t packet_len)
{
    if(packet->opcode == OPERATION_ARP_REQUEST)
    {
        
    }

    // if arp replay
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