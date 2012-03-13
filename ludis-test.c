#include <stdio.h>
#include <assert.h>

#include "common.h"
#include "ludis.h"
#include "test.h"

TEST(simple) {
    char *s;

    assert(call("PING") == LUDIS_OK);
    /*assert(format(s, "SET foo %c", 'c') == LUDIS_ERR);*/
    assert(format(s, "SET foo %b", "bar", 3) == 3);
}

int
main(void)
{
    test_simple();
    return 0;
}

