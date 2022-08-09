#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include "helpers.h"

#define MAXLINE 1024

static void
parse_arguments(int argc, char *argv[],
                int *rx_ifindex,
                int *tx_ifindex,
                unsigned char address[8])
{
    if (argc != 4) {
        printf("Wrong argument count, expected 3");
        exit(EXIT_FAILURE);
    }
    
    *rx_ifindex = (unsigned short)atoi(argv[1]);
    *tx_ifindex = (unsigned short)atoi(argv[2]);
    parse_mac(argv[3], address);
}
    
// Driver code 
int main(int argc, char *argv[]) { 
    struct sockaddr_ll server_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        .sll_halen = ETH_ALEN,
    };
    struct sockaddr_ll client_address = { 0 };
    char *hello = "Hello from server"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = &(buffer[0]) + sizeof(struct ether_header);
    int tx_ifindex;
    int sockfd; 
    int len;
    int n; 

    parse_arguments(argc, argv, &server_address.sll_ifindex, &tx_ifindex, server_address.sll_addr);

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_PACKET, SOCK_RAW, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&server_address,  
            sizeof(server_address)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    len = sizeof(client_address);
    
    if (n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_TRUNC, ( struct sockaddr *) &client_address, 
                &len) < 0)
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    } 
    buffer[n] = '\0'; 
    printf("Client : %s\n", data); 
    strcpy(data, hello);
    client_address.sll_ifindex = tx_ifindex;
    swap_mac(eh->ether_dhost, eh->ether_shost);

    len = sizeof(client_address);
    if (sendto(sockfd, (const char *)buffer, strlen(hello) + sizeof(struct ether_header),
        0, (const struct sockaddr *) &client_address, 
            len) < 0)
    {
        perror("sendto failed"); 
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n");
    close(sockfd);
    return 0; 
}
