#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "utils.h"

#define IP_ADDRESS "127.0.0.1"
#define PORT 12345
#define MAX_RETRIES 5
#define TIMEOUT_SEC 2

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servAddr;
    socklen_t slen = sizeof(servAddr);
    Packet pkt;  // For sending data (data + header)
    Header ack;  // For receiving ACK

    if (argc < 2) {
        printf("Error: No file specified.\nUsage: %s <file_name> [ip_address]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *serverIP = IP_ADDRESS; 

    if (argc >= 3) {
        serverIP = argv[2];
    }

    char *fileName = argv[1]; 
    FILE *file = fopen(fileName, "rb");
    
    if (file == NULL) {
        perror("File does not exist or cannot be opened");
        return EXIT_FAILURE;
    }

    // Creating socket file descriptor
    // Using port 0 for client so OS picks an available port
    sockfd = CreateAndBindSocket(0);

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr(serverIP);

    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
    }

    uint32_t currentSeq = 1; 
    size_t bytesRead;

    // Loop to read file and send packets
    while ((bytesRead = fread(pkt.data, 1, DEFAULT_BUFLEN, file)) > 0) {
        
        pkt.header.type = 0;       
        pkt.header.seqNum = currentSeq;
        pkt.header.dataSize = (uint32_t)bytesRead;

        int retries = 0;
        int acknowledged = 0;

        // Loop to send packet and wait for ACK with retries
        while (retries < MAX_RETRIES && !acknowledged) {
            ssize_t sentSize = sendto(sockfd, &pkt, sizeof(Packet), 0, (const struct sockaddr *)&servAddr, sizeof(servAddr));

            if (sentSize < 0) {
                perror("sendto failed");
                break; 
            }

            printf("Sent segment %u (Attempt %d)\n", pkt.header.seqNum, retries + 1);

            // Receiving only the Header (ACK) from the server
            ssize_t receivedSize = recvfrom(sockfd, &ack, sizeof(Header), 0, 
                                             (struct sockaddr *)&servAddr, &slen);

            if (receivedSize >= 0) {
                // Check if it's an ACK and if the sequence number matches
                if (ack.type == 1 && ack.seqNum == pkt.header.seqNum) {
                    printf("Received ACK for seqNum=%u\n\n", ack.seqNum);
                    acknowledged = 1;
                }
            } else {
                printf("Timeout for segment %u, retrying...\n", pkt.header.seqNum);
                retries++;
            }
        }

        // Check if sequence is acknowledged before sending next
        if (!acknowledged) {
            printf("Failed to send segment %u after %d retries\n", currentSeq, MAX_RETRIES);
            fclose(file);
            close(sockfd);
            return EXIT_FAILURE; 
        }

        currentSeq++; 
        memset(pkt.data, 0, DEFAULT_BUFLEN); // Clear buffer for next read
    }

    // Sending packet with size 0 as End of File (EOF) indicator
    pkt.header.type = 0;
    pkt.header.dataSize = 0;
    pkt.header.seqNum = currentSeq;
    sendto(sockfd, &pkt, sizeof(Packet), 0, (const struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("File transfer completed successfully.\n");

    fclose(file);
    close(sockfd);

    return EXIT_SUCCESS;
}