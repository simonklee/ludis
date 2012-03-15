#include <assert.h>

#include "common.h"
#include "test.h"
#include "handle.h"

TEST(handle_connect) {
    struct net_addr addr;
    struct ludis_handle *h;

    /* connect */
    addr = net_addr_in("127.0.0.1", 6379);

    h = handle_new();
    assert(handle_connect_addr(h, addr) == LUDIS_OK);
}

int
main(void)
{
    test_handle_connect();
    return 0;
}
