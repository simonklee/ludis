#include <stdio.h>
#include <string.h>     /* stderror */
#include <errno.h>      /* errno */ 
#include <unistd.h>     /* syscalls */
#include <sys/socket.h> /* SOCK_STREAM */

#include "addr.h"
#include "common.h"
#include "fd.h"

int
fd_connect(struct net_addr addr)
{
    int fd;

    if ((fd = socket(addr.sa_family, SOCK_STREAM, 0)) == -1)
        goto error;

    if (connect(fd, &addr.sa_addr.addr, addr.sa_addrlen) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        close(fd);
        goto error;
    }

    return fd;
error:
    return LUDIS_ERR;
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
