#ifndef LUDIS_FD_H
#define LUDIS_FD_H

#include "addr.h"
#include "common.h"

int fd_connect(struct net_addr addr);
int fd_connect_gai(int family, const char *host, int port, struct net_addr *addr);

#endif
