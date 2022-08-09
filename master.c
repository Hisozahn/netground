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

int main(int argc, char *argv[]) { 
    struct sockaddr_ll sockaddr = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(CUSTOM_PROTOCOL),
        .sll_halen = ETH_ALEN,
    };
    char *hello = "Hello from server"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = buffer + sizeof(struct ether_header);
    int sockfd; 
    int n; 

    parse_arguments(argc, argv, sockaddr.sll_addr, &sockaddr.sll_ifindex);

    if ((sockfd = socket(AF_PACKET, SOCK_RAW, 0)) < 0) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
        
    if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    if (n = recv(sockfd, (char *)buffer, MAXLINE, MSG_TRUNC) < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    } 
    buffer[n] = '\0'; 
    printf("Client : %s\n", data); 
    strcpy(data, hello);
    swap_mac(eh->ether_dhost, eh->ether_shost);
    copy_mac(eh->ether_shost, sockaddr.sll_addr);

    if (send(sockfd, buffer, strlen(hello) + sizeof(struct ether_header), 0) < 0) {
        perror("send failed"); 
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n");
    close(sockfd);
    return 0; 
}
