#include <stdio.h>
#include <assert.h>

#include "fd.h"
#include "addr.h"
#include "common.h"
#include "test.h"

TEST(gai_connect) {
    /*
    if (fd_connect_gai(&c) != 0) {
        fprintf(stderr, "fd_connect err");
        goto error;
    }*/
}

TEST(fd6_connect) {
    /*struct client c;
    struct net_addr addr;

    addr = net_addr_in6("::1", 6379);
    assert(fd_connect_addr(&c, addr) == LUDIS_OK);*/
}

TEST(http_connect) {
    int nread, fd;
    struct net_addr addr;
	char buf[IOBUFLEN];
    char *req = "GET http://simonklee.org/article/c-memory/ HTTP/1.0\n\n";

    addr = net_addr_in("178.79.132.213", 80);

    assert((fd = fd_connect_addr(addr)) != LUDIS_ERR);
    assert(fd_write(fd, req, strlen(req)) == (int)strlen(req));
    assert((nread = fd_read(fd, buf, IOBUFLEN)) > 0);

    /* close connection */
	fd_close(fd);
}

TEST(fd_connect) {
	int nread, fd;
	char buf[IOBUFLEN];
    char *req = "*1\r\n$4\r\nPING\r\n";
    struct net_addr addr;

    /* connect */
    addr = net_addr_in("127.0.0.1", 6379);
    assert((fd = fd_connect_addr(addr)) != LUDIS_ERR);
    assert(fd_write(fd, req, strlen(req)) == (int)strlen(req));
    assert((nread = fd_read(fd, buf, IOBUFLEN)) > 0);

    /* buf[nread] = '\0';
     * log_proto(buf);*/

    /* close connection */
	fd_close(fd);
}

int
main(void)
{
    test_fd_connect();
    test_fd6_connect();
    test_http_connect();
    test_gai_connect();
    return 0;
}
