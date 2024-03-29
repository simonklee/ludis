#ifndef LUDIS_FD_H
#define LUDIS_FD_H

#include "addr.h"
#include "common.h"

int fd_read(int fd, void *buf, size_t n);
int fd_write(int fd, const void *buf, size_t n);
int fd_close(int fd);

int fd_connect_addr(struct net_addr addr);
int fd_connect_gai(int family, const char *host, int port, struct net_addr *addr);

#endif
