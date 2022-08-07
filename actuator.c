#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#define MAXLINE 1024 
    
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from client"; 
    struct sockaddr_ll server_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = 0,
        .sll_ifindex = 11,
        .sll_halen = ETH_ALEN,
        .sll_addr = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc6 },
    };

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_PACKET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    int len, n; 
    
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &server_address, 
            sizeof(server_address)); 

    printf("Hello message sent.\n"); 

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &server_address, 
                NULL); 
    buffer[n] = '\0'; 
    printf("Server: %s\n", buffer); 

    close(sockfd);
    return 0; 
}
