#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAXLINE 1024 
    
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from server"; 
    sockaddr_ll client_address = { 0 };
    sockaddr_ll server_address = {
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
        
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&server_address,  
            sizeof(server_address)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    int len, n; 
    
    len = sizeof(client_address);  //len is value/result 
    
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &client_address, 
                &len); 
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &client_address, 
            len); 
    printf("Hello message sent.\n");  
        
    return 0; 
}
