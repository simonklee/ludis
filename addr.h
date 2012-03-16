#ifndef LUDIS_ADDR_H
#define LUDIS_ADDR_H

#include <netinet/in.h> /* sockaddr_in/6 */
#include <sys/socket.h> /* socklen_t */
#include <sys/un.h>     /* sockaddr_un */
#include <netdb.h>      /* struct addrinfo */

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

struct net_addr net_addr_from_in(struct sockaddr_in sa);
struct net_addr net_addr_from_in6(struct sockaddr_in6 sa);
struct net_addr net_addr_in(const char *ip, int port);
struct net_addr net_addr_in6(const char *ip, int port);
void debug_netaddr(struct net_addr *addr);
void debug_addrinfo(struct addrinfo *ai);

#endif
