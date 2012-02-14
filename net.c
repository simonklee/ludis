#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>      

#include <sys/types.h>

/* inet_pton/inet_ntop */
#include <arpa/inet.h>

#include <netdb.h>      
#include <unistd.h>

#include <sys/socket.h> /* socklen_t */
#include <netinet/in.h> /* sockaddr_in|6 */
#include <sys/un.h>     /* sockaddr_un */

#include "common.h"
#include "net.h"

struct net_addr
net_addr(struct sockaddr_in sa) 
{
    struct net_addr a;

    memset(&a, 0, sizeof(a));
    a.sa_family = sa.sin_family;
    a.sa_addrlen = sizeof(sa);
    a.sa_addr.in = sa;

    return a;
}    

struct net_addr
net_addr_in(const char *ip, int port)
{
    struct sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    inet_pton(AF_INET, ip, &sa.sin_addr);

    return net_addr(sa);
}

int
net_connect(struct client *c, struct net_addr addr)
{
    int fd;

    c->addr = addr;

    if ((fd = socket(addr.sa_family, SOCK_STREAM, 0)) == -1)
        goto error;

    if (connect(fd, &addr.sa_addr.addr, addr.sa_addrlen) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        close(fd);
        goto error;
    }

    c->fd = fd;
    return LUDIS_OK;
error:
    return LUDIS_ERR;
}

/*int
net_connect_gai(struct client *ctx)
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
        ctx->fd = s;
        break;
    }

    freeaddrinfo(servinfo);
    printf("return ok\n");
    return 0;
error:
    freeaddrinfo(servinfo);
    return 1;
}*/
