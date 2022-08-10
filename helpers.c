#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <net/if.h>

#include "helpers.h"

#define SYSFS_IFACE "/sys/class/net/"
#define SYSFS_IFACE_ADDR "/address"
#define SYSFS_IFACE_INDEX "/ifindex"
#define MAC_STR_LEN 17

void
parse_mac(const char *str, unsigned char address[ETHER_ADDR_LEN])
{
    int values[ETHER_ADDR_LEN];

    if (ETHER_ADDR_LEN == sscanf(str, "%x:%x:%x:%x:%x:%x%*c",
        &values[0], &values[1], &values[2],
        &values[3], &values[4], &values[5])) {
        int i;

        for (i = 0; i < ETHER_ADDR_LEN; ++i)
            address[i] = (unsigned char) values[i];
    } else {
        puts("Failed to parse MAC address");
        exit(EXIT_FAILURE);
    }
}

void
copy_mac(unsigned char dst[ETHER_ADDR_LEN], const unsigned char src[ETHER_ADDR_LEN])
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

void
parse_interface(const char *name, unsigned char address[ETHER_ADDR_LEN],
                int *ifindex)
{
    char index_path[sizeof(SYSFS_IFACE) + IFNAMSIZ + sizeof(SYSFS_IFACE_INDEX)];
    char addr_path[sizeof(SYSFS_IFACE) + IFNAMSIZ + sizeof(SYSFS_IFACE_ADDR)];
    FILE *index_file = NULL;
    FILE *addr_file = NULL;
    char index_str[20] = {0};
    char addr_str[MAC_STR_LEN] = {0};

    if (strlen(name) > IFNAMSIZ) {
        printf("Interface name is too long (max %d): %s\n", IFNAMSIZ, name);
        exit(EXIT_FAILURE);
    }

    if (snprintf(index_path, sizeof(index_path),
        SYSFS_IFACE "%s" SYSFS_IFACE_INDEX, name) < 0) {
        printf("snprintf failed");
        exit(EXIT_FAILURE);
    }

    if (snprintf(addr_path, sizeof(addr_path),
        SYSFS_IFACE "%s" SYSFS_IFACE_ADDR, name) < 0) {
        printf("snprintf failed");
        exit(EXIT_FAILURE);
    }

    index_file = fopen(index_path, "r");
    if (index_file == NULL) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    addr_file = fopen(addr_path, "r");
    if (addr_file == NULL) {
        perror("fopen failed");
        fclose(index_file);
        exit(EXIT_FAILURE);
    }

    if (MAC_STR_LEN != fread(addr_str, 1, MAC_STR_LEN, addr_file)) {
        perror("fread");
        fclose(index_file);
        fclose(addr_file);
        exit(EXIT_FAILURE);
    }

    if (fread(index_str, 1, sizeof(index_str), index_file) < 0) {
        perror("fread");
        fclose(index_file);
        fclose(addr_file);
        exit(EXIT_FAILURE);
    }

    parse_mac(addr_str, address);
    *ifindex = atoi(index_str);
}

void
parse_arguments(int argc, char *argv[], unsigned char address[ETHER_ADDR_LEN],
                int *ifindex)
{
    if (argc != 2) {
        printf("Wrong argument count, expected 1: interface name");
        exit(EXIT_FAILURE);
    }

    parse_interface(argv[1], address, ifindex);
}
