#include "packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345

int CreateAndBindSocket(int port) 
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
void SaveToFile(Packet* pkt, struct sockaddr_in* clientAddr) 
{
    char fileName[64];
    // Create file name based on client address
    sprintf(fileName, "received_from_%d.bin", ntohs(clientAddr->sin_port));

    FILE* file = fopen(fileName, "ab");
    
    if (file == NULL) 
    {
        perror("ERROR: Could not open file for writing");
        return;
    }

    fwrite(pkt->data, 1, pkt->header.dataSize, file);
    fclose(file);
}

void SendAck(int sockfd, uint32_t seqNum, struct sockaddr_in* clientAddr) 
{
    Header ack;
    ack.type = 1;      
    ack.seqNum = seqNum;
    ack.dataSize = 0;   

    sendto(sockfd, &ack, sizeof(Header), 0, (struct sockaddr*)clientAddr, sizeof(*clientAddr));
}
