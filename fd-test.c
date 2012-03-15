#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include <unistd.h> /* syscalls */

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
    assert(fd_connect(&c, addr) == LUDIS_OK);*/
}

TEST(http_connect) {
	int nread;
    struct client c;
    struct net_addr addr;
	char buf[IOBUFLEN];
    char *req = "GET http://simonklee.org/article/c-memory/ HTTP/1.0\n\n";

    addr = net_addr_in("178.79.132.213", 80);
    c.fd = fd_connect(addr);
    assert(c.fd != LUDIS_ERR);
    assert(write(c.fd, req, strlen(req)) >= 0);

    nread = read(c.fd, buf, IOBUFLEN);
    assert(nread >= 0);
    buf[nread] = '\0';
    /*printf(buf);*/

    /* close connection */
	close(c.fd);
}

TEST(fd_connect) {
	int nread;
    struct client c;
	char buf[IOBUFLEN];
    char *out = "*1\r\n$4\r\nPING\r\n";
    struct net_addr addr;

    /* connect */
    addr = net_addr_in("127.0.0.1", 6379);
    c.fd = fd_connect(addr);
    assert(c.fd != LUDIS_ERR);

    /* write data */
    assert(write(c.fd, out, strlen(out)) >= 0);
    /*log_proto(out); */

    /* read response */
    nread = read(c.fd, buf, IOBUFLEN);
    assert(nread >= 0);
    buf[nread] = '\0';
    /*log_proto(buf);*/

    /* close connection */
	close(c.fd);
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
