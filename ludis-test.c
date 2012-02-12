#include <stdio.h>
#include <assert.h>

#include "common.h"
#include "ludis.h"
#include "test.h"

TEST(simple) {
    assert(call("PING") == LUDIS_OK);
}

int
main(void)
{
    test_simple();
    return 0;
}

