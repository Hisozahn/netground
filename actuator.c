#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define ETHER_TYPE 0x0899 // Custom
#define ADDR_SIZE 6
    
// Driver code 
int main() { 
    const char server_addr[] = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc6 };
    const char client_addr[] = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc7 };
    struct sockaddr_ll server_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        .sll_ifindex = 13,
        .sll_halen = ETH_ALEN,
        .sll_addr = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc6 },
    };
    char *hello = "Hello from client"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = &(buffer[0]) + sizeof(struct ether_header);
    int sockfd; 
    int n;

    strcpy(data, hello);
    eh->ether_type = htons(ETHER_TYPE);
    memcpy(eh->ether_dhost, server_addr, ADDR_SIZE);
    memcpy(eh->ether_shost, client_addr, ADDR_SIZE);

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_PACKET, SOCK_RAW, 0)) < 0 ) { 
        perror("socket creation failed"); 
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

    if (n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_TRUNC, ( struct sockaddr *) &server_address, 
                NULL) < 0 )
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    } 
    buffer[n] = '\0'; 
    printf("Server: %s\n", buffer + sizeof(struct ether_header)); 

    close(sockfd);
    return 0; 
}
