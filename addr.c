#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void 
print_addrinfo(struct addrinfo *addr) 
{
	printf("addrinfo for %d\n", addr->ai_canonname);
	printf("ai_flag: %d, ai_family: %d, ai_socktype: %d, ai_protocol: %d "\
			"ai_addrlen: %d\n", addr->ai_flags, addr->ai_family, addr->ai_socktype, \
			addr->ai_protocol, addr->ai_addrlen);
			
	switch(addr->ai_family) { // IPv4
    case AF_INET: 
        {
		    char ipstr[INET_ADDRSTRLEN];
		    // inet_ntop(addr->ai_family, &(((struct sockaddr_in *)addr->ai_addr)->sin_addr), \
			ipstr, sizeof ipstr);
            struct sockaddr_in *sockaddr_in_ptr;
            struct in_addr *in_addr_ptr;
		
		    sockaddr_in_ptr = (struct sockaddr_in *)addr->ai_addr;
		    in_addr_ptr = &(sockaddr_in_ptr->sin_addr);
		    inet_ntop(addr->ai_family, in_addr_ptr, ipstr, sizeof ipstr);
		    printf("IPv4-address: %s:%d\n", ipstr, sockaddr_in_ptr->sin_port);
        }
        break;
    case AF_INET6:
        {
            char ipstr[INET6_ADDRSTRLEN];
            struct sockaddr_in6 *sockaddr_in6_ptr;
            struct in6_addr *in6_addr_ptr; // ptr to store 
            
            sockaddr_in6_ptr = (struct sockaddr_in6 *)addr->ai_addr; // addr to sockaddr_in6
            in6_addr_ptr = &(sockaddr_in6_ptr->sin6_addr); // address to struct in6_addr
            inet_ntop(addr->ai_family, in6_addr_ptr, ipstr, sizeof ipstr);
            printf("IPv6-address: %s:%d\n", ipstr, sockaddr_in6_ptr->sin6_port);
        }
        break;
	}
}
