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
#define ETHER_TYPE 0x0899 // Custom

static void
parse_arguments(int argc, char *argv[],
                int *rx_ifindex,
                int *tx_ifindex,
                unsigned char rx_address[8],
                unsigned char tx_address[8])
{
    if (argc != 5) {
        printf("Wrong argument count, expected 4");
        exit(EXIT_FAILURE);
    }
    
    *rx_ifindex = (unsigned short)atoi(argv[1]);
    *tx_ifindex = (unsigned short)atoi(argv[2]);
    parse_mac(argv[3], rx_address);
    parse_mac(argv[4], tx_address);
}
    
// Driver code 
int main(int argc, char *argv[]) { 
    struct sockaddr_ll server_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        //.sll_ifindex = 6,
        .sll_halen = ETH_ALEN,
        //.sll_addr = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc6 },
    };
    struct sockaddr_ll client_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        //.sll_ifindex = 5,
        .sll_halen = ETH_ALEN,
        //.sll_addr = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc7 },
    };
    char *hello = "Hello from client"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = &(buffer[0]) + sizeof(struct ether_header);
    int sockfd; 
    int n;

    parse_arguments(argc, argv, &client_address.sll_ifindex, &server_address.sll_ifindex,
                    client_address.sll_addr, server_address.sll_addr);

    strcpy(data, hello);
    eh->ether_type = htons(ETHER_TYPE);
    memcpy(eh->ether_dhost, server_address.sll_addr, ETHER_ADDR_LEN);
    memcpy(eh->ether_shost, client_address.sll_addr, ETHER_ADDR_LEN);

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_PACKET, SOCK_RAW, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    if ( bind(sockfd, (const struct sockaddr *)&client_address,
            sizeof(client_address)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (sendto(sockfd, (const char *)buffer, strlen(hello) + sizeof(struct ether_header),  
        0, (const struct sockaddr *) &server_address, 
            sizeof(server_address)) < 0)
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n"); 
    int len = sizeof (server_address);

    if (n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_TRUNC, ( struct sockaddr *) &server_address, 
                &len) < 0 )
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    } 
    buffer[n] = '\0'; 
    printf("Server: %s\n", data); 

    close(sockfd);
    return 0; 
}
