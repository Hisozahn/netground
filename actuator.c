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
#include "net.h"

#define MAXLINE 1024

int main(int argc, char *argv[]) { 
    char *hello = "Hello from client"; 
    char buffer[MAXLINE]; 
    struct ether_header *eh = (struct ether_header *) buffer;
    char *data = buffer + sizeof(struct ether_header);
    int sockfd; 
    int n;

    init_networking(argc, argv, &sockfd);

    strcpy(data, hello);
    eh->ether_type = htons(CUSTOM_PROTOCOL);
    copy_mac(eh->ether_dhost, g_broadcast_address);
    copy_mac(eh->ether_shost, get_own_mac());
    
    if (send(sockfd, buffer, strlen(hello) + sizeof(struct ether_header), 0) < 0) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }
    printf("Hello message sent.\n"); 

    if (n = recv(sockfd, buffer, MAXLINE,  MSG_TRUNC) < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    } 
    buffer[n] = '\0'; 
    printf("Server: %s\n", data); 

    close(sockfd);
    return 0; 
}
