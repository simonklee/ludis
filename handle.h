#ifndef LUDIS_HANDLE_H
#define LUDIS_HANDLE_H

#include "addr.h"
#include "str.h"

struct ludis_handle {
    int fd;    
    Buffer *rb;
    Buffer *wb;
};

struct ludis_handle *handle_new();
int handle_init(struct ludis_handle *h);
int handle_close(struct ludis_handle *h);
int handle_free(struct ludis_handle *h);

int handle_connect_addr(struct ludis_handle *h, const struct net_addr addr);
int handle_connect_gai(struct ludis_handle *h, int family, const char *host, int port, struct net_addr *addr);

#endif
