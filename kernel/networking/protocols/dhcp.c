#include "dhcp.h"

// computer mac address
extern uint8_t g_src_mac[6];

// contains the computer ip address
uint32_t g_self_ip;

void parse_dhcp(dhcp_packet* packet);
void dhcp_discover();


void parse_dhcp(dhcp_packet* packet)
{

}

void dhcp_discover()
{
    dhcp_packet* packet = (dhcp_packet*)kmalloc(sizeof(dhcp_packet));

    // assingning packet link layer hardware type
    packet->op_flag = 0x1;
    packet->htype_flag = 0x1;
    packet->hlen_flag = ETHERNET_HLEN;
    packet->hops_flags = 0x0;

    packet->xid = DHCP_XID;

    packet->secs = 0;
    packet->flags = 0;

    packet->ciaddr = 0;
    packet->yiaddr = 0;
    packet->siaddr = 0;
    packet->giaddr = 0;
    memcpy((void*)packet->chaddr[0], g_src_mac, sizeof(uint32_t));

    packet->magic_cookie = DHCP_MAGIC_COOKIE;

    uint8_t* curr_options = packet->options;

    // dhcp discover option
    *curr_options++ = DHCP_DISCOVER;
    *curr_options++ = 0x1;    // option size 1
    *curr_options++ = 0x1;

    // requesting 192.168.1.100
    *curr_options++ = DHCP_REQUESTED_IP_ADDRESS;
    *curr_options++ = 0x4;    // option size
    *curr_options++ = 192;
    *curr_options++ = 168;
    *curr_options++ = 1;
    *curr_options++ = 100;

    *curr_options++ = DHCP_PARAMETER_REQUEST_LIST;
    *curr_options++ = 0x4;
    *curr_options++ = 0x1;     // requesting subnet mask
    *curr_options++ = 0x3;     // requesting router ip address
    *curr_options++ = 0xf;     // domain name
    *curr_options++ = 0x6;     // domain name server
    
    // end of packet
    *curr_options++ = 0xff;     // domain name

    // sending the packet into ip broadcast
    send_udp(DHCP_CLIENT_PORT, DHCP_SERVER_PORT, sizeof(udp_packet), (void*)packet, 0xFFFFFFFF);
}