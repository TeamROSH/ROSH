#include "dhcp.h"

// computer mac address
extern uint8_t g_src_mac[6];

// contains the computer ip address
uint32_t g_self_ip;

// if there is already recived offer
bool g_reived_offer;

// is there is an ip address
bool g_is_ip;

void parse_dhcp(dhcp_packet* packet);
void dhcp_discover();
void dhcp_request(dhcp_packet* recived_packet);


void parse_dhcp(dhcp_packet* packet)
{
    // if recived ip address offer
    if(g_reived_offer == false && packet->options[2] == DHCP_OFFER)
    {
        g_reived_offer = true;
        dhcp_request(packet);

    }

    else if(packet->options[2] == DHCP_ACK)
    {
        // getting the computer ip address
        g_self_ip = packet->yiaddr;
    }

}

void dhcp_request(dhcp_packet* recived_packet)
{
    // initializing the recived offer variable
    g_reived_offer = false;

    dhcp_packet* packet = (dhcp_packet*)kmalloc(sizeof(dhcp_packet));

    // assingning packet link layer hardware type
    packet->op_flag = 0x1;
    packet->htype_flag = 0x1;
    packet->hlen_flag = ETHERNET_HLEN;
    packet->hops_flags = 0x0;

    // session id
    packet->xid = DHCP_XID;

    packet->secs = 0;
    packet->flags = 0;

    // setting ip addresses
    packet->ciaddr = recived_packet->yiaddr;
    packet->yiaddr = 0;
    packet->siaddr = recived_packet->siaddr;
    packet->giaddr = 0;
    memcpy((void*)packet->chaddr[0], g_src_mac, sizeof(uint32_t));

    packet->magic_cookie = DHCP_MAGIC_COOKIE;

    uint8_t* curr_options = packet->options;

    // dhcp request option
    *curr_options++ = DHCP_PACKET_TYPE_OPTION;
    *curr_options++ = 0x1;    // option size 1
    *curr_options++ = DHCP_REQUEST;

    uint8_t offered_ip[4] = {};

    memcpy((void*)offered_ip, (void*)recived_packet->yiaddr, sizeof(uint32_t));

    // requesting offred ip
    *curr_options++ = DHCP_REQUESTED_IP_ADDRESS;
    *curr_options++ = 0x4;    // option size
    *curr_options++ = offered_ip[0];
    *curr_options++ = offered_ip[1];
    *curr_options++ = offered_ip[2];
    *curr_options++ = offered_ip[3];

    uint8_t server_ip[4] = {};

    memcpy((void*)server_ip, (void*)recived_packet->siaddr, sizeof(uint32_t));
    
    // sending to the specific dhcp server 
    *curr_options++ = DHCP_SERVER_IP;
    *curr_options++ = 0x4;
    *curr_options++ = server_ip[0];     
    *curr_options++ = server_ip[1];     
    *curr_options++ = server_ip[2];     
    *curr_options++ = server_ip[3];     
    
    // end of packet
    *curr_options++ = 0xff;     // domain name

    // sending the packet into ip broadcast
    send_udp(DHCP_CLIENT_PORT, DHCP_SERVER_PORT, sizeof(udp_packet), (void*)packet, 0xFFFFFFFF);
}

void dhcp_discover()
{
    // initializing the recived offer variable
    g_reived_offer = false;
    g_is_ip = false;

    dhcp_packet* packet = (dhcp_packet*)kmalloc(sizeof(dhcp_packet));

    // assingning packet link layer hardware type
    packet->op_flag = 0x1;
    packet->htype_flag = 0x1;
    packet->hlen_flag = ETHERNET_HLEN;
    packet->hops_flags = 0x0;

    // session id
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
    *curr_options++ = DHCP_PACKET_TYPE_OPTION;
    *curr_options++ = 0x1;    // option size 1
    *curr_options++ = DHCP_DISCOVER;

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