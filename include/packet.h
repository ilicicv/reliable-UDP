#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define DEFAULT_BUFLEN 512

typedef struct {
    uint32_t type;      // 0 = DATA, 1 = ACK
    uint32_t seqNum;    // Sequence number 
    uint32_t dataSize;   
} Header;

typedef struct {
    Header header;          
    uint8_t data[DEFAULT_BUFLEN]; 
} Packet;

#endif