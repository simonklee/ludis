#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>  /* inet_pton/inet_ntop */
#include <netinet/in.h> /* sockaddr_in|6 */
#include <sys/un.h>     /* sockaddr_un */

#include <unistd.h>     /* syscalls */
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h> /* socklen_t */
#include <netdb.h>

#include "addr.h"
/*static void debug_addrinfo(struct addrinfo *ai);*/

struct net_addr
net_addr_from_in(struct sockaddr_in sa)
{
    struct net_addr a;

    memset(&a, 0, sizeof(a));
    a.sa_family = sa.sin_family;
    a.sa_addrlen = sizeof(sa);
    a.sa_addr.in = sa;

    return a;
}    

struct net_addr
net_addr_from_in6(struct sockaddr_in6 sa)
{
    struct net_addr a;

    memset(&a, 0, sizeof(a));
    a.sa_family = sa.sin6_family;
    a.sa_addrlen = sizeof(sa);
    a.sa_addr.in6 = sa;

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

    return net_addr_from_in(sa);
}

struct net_addr
net_addr_in6(const char *ip, int port)
{
    struct sockaddr_in6 sa;

    memset(&sa, 0, sizeof(sa));
    sa.sin6_family = AF_INET6;
    sa.sin6_port = htons(port);
    inet_pton(AF_INET6, ip, &sa.sin6_addr);

    return net_addr_from_in6(sa);
}

/*static void 
debug_addrinfo(struct addrinfo *ai)
{
	printf("addrinfo for %d\n", ai->ai_canonname);
	printf("ai_flag: %d, ai_family: %d, ai_socktype: %d, ai_protocol: %d "\
			"ai_addrlen: %d\n", ai->ai_flags, ai->ai_family, ai->ai_socktype, \
			ai->ai_protocol, ai->ai_addrlen);
			
	switch(ai->ai_family) {
    case AF_INET: 
        {
		    char ipstr[INET_ADDRSTRLEN];
		    // inet_ntop(ai->ai_family, &(((struct sockaddr_in *)ai->ai_addr)->sin_addr),
			// ipstr, sizeof ipstr);
            struct sockaddr_in *sockaddr_in_ptr;
            struct in_addr *in_addr_ptr;
		
		    sockaddr_in_ptr = (struct sockaddr_in *)ai->ai_addr;
		    in_addr_ptr = &(sockaddr_in_ptr->sin_addr);
		    inet_ntop(ai->ai_family, in_addr_ptr, ipstr, sizeof ipstr);
		    printf("IPv4-address: %s:%d\n", ipstr, sockaddr_in_ptr->sin_port);
        }
        break;
    case AF_INET6:
        {
            char ipstr[INET6_ADDRSTRLEN];
            struct sockaddr_in6 *sockaddr_in6_ptr;
            struct in6_addr *in6_addr_ptr; // ptr to store
            
            sockaddr_in6_ptr = (struct sockaddr_in6 *)ai->ai_addr; // addr to sockaddr_in6 
            in6_addr_ptr = &(sockaddr_in6_ptr->sin6_addr); // address to struct in6_addr 
            inet_ntop(ai->ai_family, in6_addr_ptr, ipstr, sizeof ipstr);
            printf("IPv6-address: %s:%d\n", ipstr, sockaddr_in6_ptr->sin6_port);
        }
        break;
	}
}*/
