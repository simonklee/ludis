#include <stdio.h>
#include <assert.h>

#include "addr.h"
#include "common.h"
#include "test.h"

TEST(ip6_addr) {
    /*struct client c;
    struct net_addr addr;

    addr = net_addr_in6("::1", 6379);
    assert(net_connect(&c, addr) == LUDIS_OK);
    */
}

TEST(ip4_addr) {
    /*struct net_addr addr;
    addr = */net_addr_in("127.0.0.1", 6379);
}

int
main(void)
{
    test_ip4_addr();
    test_ip6_addr();
    return 0;
}
