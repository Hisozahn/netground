#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "helpers.h"

void
parse_mac(const char *str, unsigned char address[8])
{
    int values[ETHER_ADDR_LEN];

    if (ETHER_ADDR_LEN == sscanf(str, "%x:%x:%x:%x:%x:%x%*c",
        &values[0], &values[1], &values[2],
        &values[3], &values[4], &values[5])) {
        int i;

        for (i = 0; i < ETHER_ADDR_LEN; ++i)
            address[i] = (unsigned char) values[i];
    } else {
        printf("Failed to parse MAC address");
        exit(EXIT_FAILURE);
    }
}

void
copy_mac(unsigned char dst[ETHER_ADDR_LEN], unsigned char src[ETHER_ADDR_LEN])
{
    memcpy(dst, src, ETHER_ADDR_LEN);
}

void
swap_mac(unsigned char mac1[ETHER_ADDR_LEN], unsigned char mac2[ETHER_ADDR_LEN])
{
    unsigned char tmp[ETHER_ADDR_LEN];

    copy_mac(tmp, mac1);
    copy_mac(mac1, mac2);
    copy_mac(mac2, tmp);
}
