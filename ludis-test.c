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
    Str *s;

    va_start(ap, fmt);
    ret = vformat(&s, fmt, ap);
    va_end(ap);

    if (exp == NULL) {
        assert(ret == LUDIS_ERR);
        return LUDIS_OK;
    } 

    assert(ret == (int)strlen(exp));
    for (i = 0; i < (int)strlen(exp); i++) {
        if (s->data[i] != exp[i]) {
            printf("s[%d]: %c != exp[%d]: %c\n", i, s->data[i], i, exp[i]);
            assert(s->data[i] == exp[i]);
        }
    }

    str_free(s);
    return LUDIS_OK;
}

TEST(simple) {
    format_test(NULL, "SET foo %c", 'c');
    format_test(NULL, "%c", 'c');
    format_test(NULL, "%c");

    format_test("*2\r\n$4\r\nPING\r\n$4\r\nPONG\r\n", "PING PONG");
    format_test("*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$3\r\nbar\r\n", "SET foo %b", "bar", 3);
    format_test("*3\r\n$3\r\nSET\r\n$3\r\nbar\r\n$3\r\nfoo\r\n", "SET %b foo", "bar", 3);
    format_test("*3\r\n$3\r\nSET\r\n$3\r\nbar\r\n$3\r\nfoo\r\n", "  SET    %b   foo   ", "bar", 3);
    format_test("*2\r\n$3\r\nGET\r\n$3\r\nfoo\r\n", "GET %s", "foo");
    format_test("*6\r\n$3\r\nGET\r\n$1\r\na\r\n$1\r\nb\r\n$1\r\nc\r\n$1\r\nd\r\n$1\r\ne\r\n", 
            "GET %b %b %b %b %b", "a", 1, "b", 1, "c", 1, "d", 1, "e", 1);

    assert(int_len(1) == 1);
    assert(int_len(9) == 1);
    assert(int_len(10) == 2);
    assert(int_len(199) == 3);
    assert(int_len(536870912) == 9);
}

int
main(void)
{
    test_simple();
    return 0;
}

