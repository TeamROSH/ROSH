#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

typedef struct ethernet_header{
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uint8_t ethernet_type[2];
}ethernet_header;

#endif