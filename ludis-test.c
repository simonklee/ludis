#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "common.h"
#include "ludis.h"
#include "test.h"
#include "str.h"

static int
format_test(const char *exp, const char *fmt, ...) {
    va_list ap;
    int ret, i;
    Str s;

    va_start(ap, fmt);
    ret = vformat(&s, fmt, ap);
    va_end(ap);

    if (exp == NULL) {
        assert(ret == LUDIS_ERR);
        return LUDIS_OK;
    } 

    assert(ret == (int)strlen(exp));

    for (i = 0; i < (int)strlen(exp); i++) {
        if (s[i] != exp[i]) {
            printf("s[%d]: %c != exp[%d]: %c\n", i, s[i], i, exp[i]);
            assert(s[i] == exp[i]);
        }
    }

    str_free(s);
    return LUDIS_OK;
}

TEST(simple) {
    format_test(NULL, "SET foo %c", 'c');
    format_test(NULL, "%c", 'c');
    format_test(NULL, "%c");

    format_test("*2\r\n$2\r\nPING\r\n$2\r\nPONG\r\n", "PING PONG");
    format_test("*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n", "SET foo %b", "bar", 3);
    format_test("*3\r\n$3\r\nSET\r\n$3\r\nbar\r\n$3\r\nfoo\r\n", "SET %b foo", "bar", 3);

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

