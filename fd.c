#include <stdio.h>
#include <string.h>     /* stderror */
#include <errno.h>      /* errno */ 
#include <unistd.h>     /* syscalls */
#include <sys/socket.h> /* SOCK_STREAM */

#include "addr.h"
#include "common.h"
#include "fd.h"

int 
fd_read(int fd, void *buf, size_t n)
{
    int nread;

    nread = read(fd, buf, n);

    if (nread ==  -1)
        return LUDIS_ESYS;

    if (nread ==  0)
        return LUDIS_EEOF;

    return nread;
}

int 
fd_write(int fd, const void *buf, size_t n)
{
    int nwrite;

    nwrite = write(fd, buf, n);
    
    if (nwrite ==  -1)
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
    int fd;

    if ((fd = socket(family, SOCK_STREAM, 0)) == -1)
        goto error;

    if (connect(fd, addr, addrlen) == -1) {
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

/*int
fd_connect_gai(int family, const char *host, int port, struct net_addr *addr)
{
    int s, r;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((r = getaddrinfo("127.0.0.1", "6379", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
        return 1;
    }

    printf("addr ok\n");

    for (p = servinfo; p != NULL; p = servinfo->ai_next) {
        if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        printf("socket ok\n");

        if (connect(s, p->ai_addr, p->ai_addrlen) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            close(s);
            goto error;
        }

        printf("connect ok\n");
        c->fd = s;
        break;
    }

    freeaddrinfo(servinfo);
    printf("return ok\n");
    return 0;
error:
    freeaddrinfo(servinfo);
    return 1;
}*/
