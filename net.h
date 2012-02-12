#ifndef _NET_H
#define _NET_H

#include <sys/socket.h> /* socklen_t */
#include <netinet/in.h> /* sockaddr_in/6 */
#include <sys/un.h>     /* sockaddr_un */

#define NET_READSIZE 1024 * 16

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

struct net_addr net_addr(struct sockaddr_in sa);
struct net_addr net_addr_in(const char *ip, int port);
int net_connect(struct client *c, struct net_addr addr);
int net_connect_gai(struct client *c);

#endif
