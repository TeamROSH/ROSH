#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include "../../memory/heap.h"
#include "../../../libc/memory.h"
#include "../../../libc/string.h"
#include "ethernet.h"

#define ETHERNET_HLEN 6
#define IPV4_PLEN 4

#define OPERATION_ARP_REQUEST 1
#define OPERATION_ARP_REPLAY 2

#define ARP_CACHE_LEN 256

typedef struct arp_packet
{
    uint16_t htype; // Hardware type
    uint16_t ptype; // Protocol type
    uint8_t  hlen; // Hardware address length (Ethernet = 6)
    uint8_t  plen; // Protocol address length (IPv4 = 4)
    uint16_t opcode; // ARP Operation Code
    uint8_t srchw[ETHERNET_HLEN]; // Source hardware address 
    uint32_t  srcpr; // Source protocol address 
    uint8_t  dsthw[ETHERNET_HLEN]; // Destination hardware address 
    uint32_t  dstpr; // Destination protocol address 
}arp_packet;

typedef struct device_address{
    uint8_t mac_address[6];
    uint32_t ip_address;
}device_address;

/*
    This function parses an arp packet
    if arp request sends and replay
    if arp replay parses the replay
    @param packet : the arp packet
    @param packet_len: the arp packet len
*/
void parse_arp_packet(arp_packet* packet, uint32_t packet_len);

/*
    This function finds the index of device in the arp cache
    @param device : the device address
    returns the index pf the device if found and -1 if not
*/
int find_arp_device(device_address* device);

/*
    This finds the mac address of network device in the arp cache
    @param ip_address: the device ip address
    retruns the mac address or null if not found
*/
uint8_t* find_mac_via_ip(uint32_t ip_address);

/*
    This function creates an arp packet according to the parameters and sends it
    @param src_ip : the source ip
    @param dst_ip : the destination ip
    @param src_mac: the source mac
    @param dst_mac: the destination mac
    @param opcode : the packet opcode
*/
void create_and_send_arp(uint32_t src_ip, uint32_t dest_ip, uint8_t src_mac[6], uint8_t dst_mac[6], uint16_t opcode);

/*
    This function sends an arp request for specific ip address
    @param dst_ip: the requested ip
*/
void send_arp(uint32_t dst_ip);
#endif
