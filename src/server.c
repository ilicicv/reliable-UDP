#include <stdio.h>      //printf, perror
#include <stdlib.h>     //EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>     //memset
#include <sys/socket.h> //socket, bind, recvfrom, sendto
#include <arpa/inet.h>  //struct sockaddr_in
#include <unistd.h>     //close
#include "utils.h"

#define PORT 12345

int main()
{
    int return_value = EXIT_SUCCESS;
    int server_socket_fd;
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    Packet pkt_received;

    server_socket_fd = CreateAndBindSocket(PORT);

    printf("Server bound to port %d and listening...\n", PORT);

    while (1)
    {
        printf("\nWaiting for data...\n");
        
        ssize_t received_size;
        
        // Reset memory for incoming packet
        memset(&pkt_received, 0, sizeof(Packet));
        
        if ((received_size = recvfrom(server_socket_fd, &pkt_received, sizeof(Packet), 0, (struct sockaddr *)&cliaddr, &len)) >= 0)
        {   
            printf("-------------------------------------------\n");
            printf("Received message: %s\n", pkt_received.data);
            printf("Sequence number: %u\n", pkt_received.header.seqNum);
            printf("From: %s:%hu\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

            // Save received data to file
            SaveToFile(&pkt_received, &cliaddr);
            printf("File status: Data appended to file successfully.\n");

            // Send ACK to client
            SendAck(server_socket_fd, pkt_received.header.seqNum, &cliaddr);
            printf("ACK Status: Sent ACK for packet #%u\n", pkt_received.header.seqNum);
            printf("-------------------------------------------\n");
        }
        else
        {
            perror("recvfrom failed");
            usleep(100000); // Sleep for 100 milliseconds before retrying   
        }
    }

    close(server_socket_fd);
    
    return return_value;
}
