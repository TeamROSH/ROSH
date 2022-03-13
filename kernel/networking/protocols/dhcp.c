#include "dhcp.h"
#include "arp.h"
#include "ethernet.h"
#include "udp.h"

// computer mac address
extern uint8_t g_src_mac[6];

extern device_address g_address_cache[ARP_CACHE_LEN];

// contains the computer ip address
uint32_t g_self_ip = 0;

// if there is already recived offer
uint8_t g_reived_offer;

// is there is an ip address
uint8_t g_is_ip;

void parse_dhcp(dhcp_packet* packet);
void dhcp_discover();
void dhcp_request(dhcp_packet* recived_packet);


void parse_dhcp(dhcp_packet* packet)
{
    // if recived ip address offer
    if(g_is_ip == 0 && g_reived_offer == 0 && packet->options[2] == DHCP_OFFER)
    {
        g_reived_offer = 1;
        dhcp_request(packet);
    }

    else if(packet->options[2] == DHCP_ACK)
    {
        // getting the computer ip address
        g_self_ip = packet->yiaddr;
        g_is_ip = 1;

		// saving to arp cache
        for(int i = 0; i < ARP_CACHE_LEN; i++)
        {  
            // if saving the device address
            if(g_address_cache[i].ip_address == 0)
            {
                g_address_cache[i].ip_address = g_self_ip;
                memcpy(g_address_cache[i].mac_address, g_src_mac, 6);
                break;
            }
        }
    }

    kfree(packet);

}

void dhcp_request(dhcp_packet* recived_packet)
{
    // initializing the recived offer variable
    g_reived_offer = 0;

    dhcp_packet packet;
	memset(&packet, 0, sizeof(dhcp_packet));

    // assingning packet link layer hardware type
    packet.op_flag = 0x1;
    packet.htype_flag = 0x1;
    packet.hlen_flag = ETHERNET_HLEN;
    packet.hops_flags = 0x0;

    // session id
    packet.xid = DHCP_XID;

    packet.secs = 0;
    packet.flags = 0;

    // setting ip addresses
    packet.ciaddr = 0;
    packet.yiaddr = 0;
    packet.siaddr = 0;
    // packet.siaddr = recived_packet->siaddr;
    packet.giaddr = 0;
    memcpy(packet.chaddr, g_src_mac, 6);

    packet.magic_cookie = DHCP_MAGIC_COOKIE;

    uint8_t* curr_options = packet.options;

    // dhcp request option
    *curr_options++ = DHCP_PACKET_TYPE_OPTION;
    *curr_options++ = 0x1;    // option size 1
    *curr_options++ = DHCP_REQUEST;

    uint8_t offered_ip[4] = {};

    memcpy((void*)offered_ip, &(recived_packet->yiaddr), sizeof(uint32_t));

    // requesting offred ip
    *curr_options++ = DHCP_REQUESTED_IP_ADDRESS;
    *curr_options++ = 0x4;    // option size
    *curr_options++ = offered_ip[0];
    *curr_options++ = offered_ip[1];
    *curr_options++ = offered_ip[2];
    *curr_options++ = offered_ip[3];

    uint8_t server_ip[4] = {};

    memcpy((void*)server_ip, &(recived_packet->siaddr), sizeof(uint32_t));

	*curr_options++ = 0x3D;
    *curr_options++ = 0x7;
    *curr_options++ = 1;     
    for (int i = 0; i < 6; i++)
		*curr_options++ = g_src_mac[i];  
	
	*curr_options++ = 0x0C;		// name
	*curr_options++ = 4;		// len
	*curr_options++ = 'R';
	*curr_options++ = 'O';
	*curr_options++ = 'S';
	*curr_options++ = 'H';

	*curr_options++ = DHCP_PARAMETER_REQUEST_LIST;
    *curr_options++ = 0x0D;
    *curr_options++ = 0x1;     // requesting subnet mask
    *curr_options++ = 0x1C;
    *curr_options++ = 0x2;
    *curr_options++ = 0x3;
    *curr_options++ = 0xF;
    *curr_options++ = 0x6;
    *curr_options++ = 0x77;
    *curr_options++ = 0x0C;
    *curr_options++ = 0x2C;
    *curr_options++ = 0x2F;
    *curr_options++ = 0x1A;
    *curr_options++ = 0x79;
    *curr_options++ = 0x2A;
    
    // end of packet
    *curr_options++ = 0xff;     // domain name

    // sending the packet into ip broadcast
    send_udp(DHCP_CLIENT_PORT, DHCP_SERVER_PORT, sizeof(dhcp_packet), &packet, 0xFFFFFFFF);
}

void dhcp_discover()
{
    // initializing the recived offer variable
    g_reived_offer = 0;
    g_is_ip = 0;

    dhcp_packet packet;
	memset(&packet, 0, sizeof(dhcp_packet));

    // assingning packet link layer hardware type
    packet.op_flag = 0x1;
    packet.htype_flag = 0x1;
    packet.hlen_flag = ETHERNET_HLEN;
    packet.hops_flags = 0x0;

    // session id
    packet.xid = DHCP_XID;

    packet.secs = 0;
    packet.flags = 0;

    packet.ciaddr = 0;
    packet.yiaddr = 0;
    packet.siaddr = 0;
    packet.giaddr = 0;
    memcpy(packet.chaddr, g_src_mac, 6);

    packet.magic_cookie = DHCP_MAGIC_COOKIE;

    uint8_t* curr_options = packet.options;

    // dhcp discover option
    *curr_options++ = DHCP_PACKET_TYPE_OPTION;
    *curr_options++ = 0x1;    // option size 1
    *curr_options++ = DHCP_DISCOVER;

    // requesting R.O.S.H
    *curr_options++ = DHCP_REQUESTED_IP_ADDRESS;
    *curr_options++ = 0x4;    // option size
    *curr_options++ = 'R';
	*curr_options++ = 'O';
	*curr_options++ = 'S';
	*curr_options++ = 'H';

	*curr_options++ = 0x0C;		// name
	*curr_options++ = 4;		// len
	*curr_options++ = 'R';
	*curr_options++ = 'O';
	*curr_options++ = 'S';
	*curr_options++ = 'H';

    *curr_options++ = DHCP_PARAMETER_REQUEST_LIST;
    *curr_options++ = 0x0D;
    *curr_options++ = 0x1;     // requesting subnet mask
    *curr_options++ = 0x1C;
    *curr_options++ = 0x2;
    *curr_options++ = 0x3;
    *curr_options++ = 0xF;
    *curr_options++ = 0x6;
    *curr_options++ = 0x77;
    *curr_options++ = 0x0C;
    *curr_options++ = 0x2C;
    *curr_options++ = 0x2F;
    *curr_options++ = 0x1A;
    *curr_options++ = 0x79;
    *curr_options++ = 0x2A;
    
    // end of packet
    *curr_options++ = 0xff;

    // sending the packet into ip broadcast
    send_udp(DHCP_CLIENT_PORT, DHCP_SERVER_PORT, sizeof(dhcp_packet), &packet, 0xFFFFFFFF);
}

void print_net_info()
{
	puts("IP address: ");
	puti(((uint8_t*)&(g_self_ip))[0]);
	for (int i = 1; i < 4; i++)
	{
		putc('.'); puti(((uint8_t*)&(g_self_ip))[i]);
	}

	puts("\nMAC address: ");
	putc(hex(g_src_mac[0] / 16)); putc(hex(g_src_mac[0] % 16));
	for (int i = 1; i < 6; i++)
	{
		putc(':'); putc(hex(g_src_mac[i] / 16)); putc(hex(g_src_mac[i] % 16));
	}
}