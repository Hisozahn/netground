#ifndef __NET_H__
#define __NET_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#define CUSTOM_PROTOCOL 0x0899

extern const unsigned char g_broadcast_address[];

void init_networking(int argc, char *argv[], int *sockfd);
const unsigned char * get_own_mac();

#endif
