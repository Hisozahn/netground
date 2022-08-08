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
    
// Driver code 
int main() { 
    struct sockaddr_ll server_address = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(ETH_P_ALL),
        .sll_ifindex = 4,
        .sll_halen = ETH_ALEN,
        .sll_addr = { 0x62, 0xcd, 0xaa, 0x7a, 0x17, 0xc6 }, // 62:cd:aa:7a:17:c6
    };
    struct sockaddr_ll client_address = { 0 };
    char *hello = "Hello from server"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = &(buffer[0]) + sizeof(struct ether_header);
    int sockfd; 
    int len;
    int n; 

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
    
    len = sizeof(client_address);  //len is value/result 
    
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
    client_address.sll_ifindex = 6; // Bridge
    char s = eh->ether_dhost[5];
    eh->ether_dhost[5] = eh->ether_shost[5];
    eh->ether_shost[5] = s;

    len = sizeof(client_address);  //len is value/result 

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
