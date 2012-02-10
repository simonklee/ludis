#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>      

#include <sys/socket.h>
#include <sys/types.h>

/* inet_pton/inet_ntop */
#include <arpa/inet.h>

#include <netdb.h>      
#include <unistd.h>

#include <netinet/in.h>
#include <sys/un.h>

#define PORT 6379
#define MAXDATASIZE 100

enum {
    NET_CONNECTED
};

struct net_addr {
    int sa_family;
    socklen_t sa_addrlen;

    union {
        struct sockaddr addr;
        struct sockaddr_in in;
        struct sockaddr_in6 in6;
        struct sockaddr_un un;
    } sa_addr;
};

struct client {
    int fd;
    int flags;
    struct net_addr addr;
};

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

    printf("socket ok\n");

    if (connect(fd, &addr.sa_addr.addr, addr.sa_addrlen) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        close(fd);
        goto error;
    }

    printf("connect ok\n");
    c->fd = fd;
    c->flags |= NET_CONNECTED;

    return 0;
error:
    return 1;
}

int
net_connect_gai(struct client *ctx)
{
    int s, r;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
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
        ctx->flags |= NET_CONNECTED;
        break;
    }

    freeaddrinfo(servinfo);
    printf("return ok\n");
    return 0;
error:
    freeaddrinfo(servinfo);
    return 1;
}

int 
main(void) 
{
	int nread, i;
    struct client c;
	char buf[MAXDATASIZE];
    /*char *out = "*2\r\n$3\r\nGET\r\n$3\r\nfoo\r\n";*/
    char *out = "*1\r\n$4\r\nPING\r\n";
    struct net_addr addr;

    addr = net_addr_in("127.0.0.1", 6379);

    if (net_connect(&c, addr) != 0) {
        fprintf(stderr, "net_connect err");
        goto error;
    }

    /*if (net_connect_gai(&c) != 0) {
        fprintf(stderr, "net_connect err");
        goto error;
    }*/

    for (i = 0; i < 1; i++) {
        write(c.fd, out, strlen(out));
        if ((nread=recv(c.fd, buf, MAXDATASIZE, 0)) == -1) { 
            fprintf(stderr, "recv() error\n");
            goto error;
        }
        buf[nread] = '\0';
        printf("Server Message: %s", buf);
    }

	close(c.fd);
    return 0;
error:
    close(c.fd);
    return 1;
}
