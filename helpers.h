#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <net/ethernet.h> 

void parse_mac(const char *str, unsigned char address[8]);
void swap_mac(unsigned char mac1[ETHER_ADDR_LEN], unsigned char mac2[ETHER_ADDR_LEN]);
void copy_mac(unsigned char dst[ETHER_ADDR_LEN], unsigned char src[ETHER_ADDR_LEN]);

#endif
