#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "str.h"

/*void
test_ludis(void) 
{
    struct query *q;
    int i;

    q = query_new();
    q->buf[0] = '\0';
    q->len = 1;
    query_write(q, "empty");

    if (query_read(q, "query.rp") != LUDIS_OK) {
        fprintf(stderr, "err\n");
        goto error;
    }
    
    for (i = 0; i < q->len; i++)
        debug_chr(q->buf[i]);
    
    query_free(q);
error:
    if (q) query_free(q);
}*/

TEST(str) {
    char *s;
    int m;

    s = str_new(16);
    assert(m = (str_len(s)) == 0);

    /*n = buffer_read(b, s);

    test("buffer_read ", n == m);

    for (i = 0; i < n; i++)
        assert(b->buf[i] == s[i]);

    free(s);*/
    str_free(s);
}

int 
main(void) 
{
    test_str();
    return 0;
}
