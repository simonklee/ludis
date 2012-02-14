#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

/* read/write/close */
#include <unistd.h>

#include "net.h"
#include "common.h"
#include "test.h"

TEST(net_gai_connect) {
    /*if (net_connect_gai(&c) != 0) {
        fprintf(stderr, "net_connect err");
        goto error;
    }*/
}

TEST(net_connect) {
	int nread;
    struct client c;
	char buf[NET_READSIZE];
    char *out = "*1\r\n$4\r\nPING\r\n";
    struct net_addr addr;

    /* connect */
    addr = net_addr_in("127.0.0.1", 6379);
    assert(net_connect(&c, addr) == LUDIS_OK);

    /* write data */
    assert(write(c.fd, out, strlen(out)) >= 0);
    log_proto(out);

    /* read response */
    nread = read(c.fd, buf, NET_READSIZE);
    assert(nread >= 0);
    buf[nread] = '\0';
    log_proto(buf);

    /* close connection */
	close(c.fd);
}

int
main(void)
{
    test_net_connect();
    return 0;
}
