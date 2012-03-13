#include <stdio.h>
#include <assert.h>

#include "common.h"
#include "ludis.h"
#include "test.h"
#include "str.h"

TEST(simple) {
    int i;
    char *s;
    const char *exp = "*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n";

    /*assert(format(&s, "SET foo %c", 'c') == LUDIS_ERR);
    assert(format(&s, "%c", 'c') == LUDIS_ERR);*/
    assert(format(&s, "PING") == 5);
    /*assert(format(&s, "SET foo %b", "bar", 3) == 31);

    for (i = 0; i < 32; i++) {
        assert(s[i] == exp[i]);
    }*/

    str_free(s);

    assert(int_len(1) == 1);
    assert(int_len(9) == 1);
    assert(int_len(10) == 2);
    assert(int_len(199) == 3);
    printf("%d\n", int_len(536870912));
    assert(int_len(536870912) == 9);
}

int
main(void)
{
    test_simple();
    return 0;
}

