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
