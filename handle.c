#include <stdio.h>
#include <stdlib.h>

#include "fd.h"
#include "handle.h"
#include "lmalloc.h"
#include "str.h"

struct ludis_handle *
handle_new()
{
    struct ludis_handle *h;

    h = lmalloc(sizeof(struct ludis_handle));
    handle_init(h);
    return h;
}

int 
handle_init(struct ludis_handle *h)
{
    h->fd = -1;
    h->rb = NULL;
    h->wb = NULL;
    return LUDIS_OK;
}

int 
handle_free(struct ludis_handle *h) 
{
    if (handle_close(h) != LUDIS_OK)
        return LUDIS_ERR;

    free(h);
    return LUDIS_OK;
}

int 
handle_close(struct ludis_handle *h)
{
    if (fd_close(h->fd) == LUDIS_OK)
        h->fd = -1;
    else 
        return LUDIS_ERR;

    if (h->rb) {
        buffer_free(h->rb);
        h->rb = NULL;
    }

    if (h->wb) {
        buffer_free(h->wb);
        h->wb = NULL;
    }

    return LUDIS_OK;
}

int 
handle_connect_addr(struct ludis_handle *h, const struct net_addr addr)
{
    int fd;

    if ((fd = fd_connect_addr(addr)) < 0)
        return fd;

    h->fd = fd;
    h->rb = buffer_new(0);
    h->wb = buffer_new(0);

    return LUDIS_OK;
}

int 
handle_connect_gai(struct ludis_handle *h, int family, const char *host, 
                   int port, struct net_addr *addr) 
{
    int fd;

    if ((fd = fd_connect_gai(family, host, port, addr)) < 0)
        return fd;

    h->fd = fd;
    h->rb = buffer_new(0);
    h->wb = buffer_new(0);

    return LUDIS_OK;
}
