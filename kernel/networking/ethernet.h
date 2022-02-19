#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

#define HEADER_TYPE_IP  0x0800
#define HEADER_TYPE_ARP 0x0806

typedef struct ethernet_header{
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uint8_t ethernet_type[2];
}ethernet_header;

#endif