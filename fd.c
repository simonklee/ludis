#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* stderror */
#include <errno.h>      /* errno */ 
#include <unistd.h>     /* syscalls */
#include <sys/socket.h> /* SOCK_STREAM */
#include <sys/types.h>  /* SOCK_STREAM */
#include <netdb.h>      /* addrinfo, sockaddr */
#include <netinet/tcp.h> /* IPPROTO_TCP/TCP_NODELAY */

#include "addr.h"
#include "common.h"
#include "fd.h"

int 
fd_read(int fd, void *buf, size_t n)
{
    int nread;

    do {
        nread = read(fd, buf, n);
    } while (nread == -1 && errno == EINTR);

    if (nread == -1)
        return LUDIS_ESYS;

    if (nread == 0)
        return LUDIS_EEOF;

    return nread;
}

int 
fd_write(int fd, const void *buf, size_t n)
{
    int nwrite;

    nwrite = write(fd, buf, n);
    
    if (nwrite == -1)
        return LUDIS_ESYS;

    return nwrite;
}

int 
fd_close(int fd)
{
    if (fd >= 0 && close(fd) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return LUDIS_ERR;
    }

    return LUDIS_OK;
}

static int
fd_connect(int family, const struct sockaddr *addr, socklen_t addrlen)
{
    int fd, yes = 1;

    if ((fd = socket(family, SOCK_STREAM, 0)) == -1)
        goto error;

    if (connect(fd, addr, addrlen) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        goto error;
    }

    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        goto error;
    }

    return fd;
error:
    fd_close(fd);
    return LUDIS_ERR;
}

int
fd_connect_addr(struct net_addr addr)
{
    return fd_connect(addr.sa_family, &addr.sa_addr.addr, addr.sa_addrlen);
}

int
fd_connect_gai(int family, const char *host, int port, struct net_addr *addr)
{
    int fd, rv;
    char buf[6];
    struct addrinfo hints, *servinfo, *p;
   
    snprintf(buf, 6, "%d", port);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, buf, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo err: `%s:%d` %s\n", host, port, gai_strerror(rv));
        return LUDIS_EGAI;
    }

    if (servinfo == NULL)
        goto error;

    for (p = servinfo; p != NULL; p = p->ai_next) {
        fd = fd_connect(p->ai_family, p->ai_addr, p->ai_addrlen);

        if (fd == -1)
            goto error;

        memset(addr, 0, sizeof(*addr));
        memcpy(&addr->sa_addr, p->ai_addr, p->ai_addrlen);
        addr->sa_family = p->ai_family;
        addr->sa_addrlen = p->ai_addrlen;
        freeaddrinfo(servinfo);
        return fd;
    }
error:
    freeaddrinfo(servinfo);
    return LUDIS_ESYS;
}
