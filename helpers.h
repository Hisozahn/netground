#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <net/ethernet.h> 

extern const unsigned char g_broadcast_address[];

void parse_mac(const char *str, unsigned char address[ETHER_ADDR_LEN]);
void swap_mac(unsigned char mac1[ETHER_ADDR_LEN], unsigned char mac2[ETHER_ADDR_LEN]);
void copy_mac(unsigned char dst[ETHER_ADDR_LEN], const unsigned char src[ETHER_ADDR_LEN]);
void parse_interface(const char *name, unsigned char address[ETHER_ADDR_LEN],
                     int *ifindex);
void parse_arguments(int argc, char *argv[],  unsigned char address[ETHER_ADDR_LEN],
                     int *ifindex);

#endif
