#ifndef _NET_H
#define _NET_H

#include <sys/socket.h> /* socklen_t */
#include <netinet/in.h> /* sockaddr_in/6 */
#include <sys/un.h>     /* sockaddr_un */

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
    struct net_addr addr;
};

#endif
