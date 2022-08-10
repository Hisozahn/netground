#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <net/if.h>

#include "net.h"
#include "helpers.h"

const unsigned char g_broadcast_address[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static unsigned char own_mac[ETHER_ADDR_LEN] = {0};
static int mac_init = 0;

void
init_networking(int argc, char *argv[], int *sockfd)
{
    struct sockaddr_ll sockaddr = {
        .sll_family = AF_PACKET,
        .sll_protocol = htons(CUSTOM_PROTOCOL),
        .sll_halen = ETH_ALEN,
    };

    if (mac_init) {
        puts("networking is already init");
        exit(EXIT_FAILURE);
    }

    parse_arguments(argc, argv, sockaddr.sll_addr, &sockaddr.sll_ifindex);
    copy_mac(own_mac, sockaddr.sll_addr);

    if ((*sockfd = socket(AF_PACKET, SOCK_RAW, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (bind(*sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    mac_init = 1;
}

const unsigned char *
get_own_mac()
{
    return own_mac;
}
