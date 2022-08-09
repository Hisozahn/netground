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


// Driver code 
int main(int argc, char *argv[]) { 
    struct sockaddr_ll client_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        .sll_halen = ETH_ALEN,
    };
    char *hello = "Hello from client"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = &(buffer[0]) + sizeof(struct ether_header);
    int sockfd; 
    int n;

    parse_arguments(argc, argv, client_address.sll_addr, &client_address.sll_ifindex);

    strcpy(data, hello);
    eh->ether_type = htons(ETHER_TYPE);
    copy_mac(eh->ether_dhost, g_broadcast_address);
    copy_mac(eh->ether_shost, client_address.sll_addr);

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
        0, (const struct sockaddr *) &client_address, 
            sizeof(client_address)) < 0)
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n"); 
    int len = sizeof (client_address);

    if (n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_TRUNC, ( struct sockaddr *) &client_address, 
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
