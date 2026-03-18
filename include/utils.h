#ifndef UTILS_H
#define UTILS_H

#include "packet.h"
#include <arpa/inet.h>

int CreateAndBindSocket(int port);
void SaveToFile(Packet* pkt, struct sockaddr_in* clientAddr);
void SendAck(int sockfd, uint32_t seqNum, struct sockaddr_in* clientAddr);

#endif