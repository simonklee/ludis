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

TEST(buf) {
    struct buffer *b;
    char p[16];
    /*int m, nread, i;*/

    b = buffer_new(0);

    assert(buffer_len(b)== 0);
    assert(buffer_read(b, p, 16)== EOF);
    assert(buffer_reads(b, p) == EOF);

    /*
    printf("%d == %d\n", m, nread);
    assert(nread == m);

    for (i = 0; i < nread; i++)
        assert(b->s[i] == s[i]);

    */
    buffer_free(b);
}


TEST(str) {
    str s;
    char p[16];

    s = str_new(16);

    assert(str_len(s) == 0);
    assert(str_avail(s) == 16);

    sprintf(p, "hello");
    str_appends(s, p);

    assert(str_len(s) == (int)strlen(p));
    assert(str_avail(s) == str_cap(s) - (int)strlen(p));

    str_free(s);
}

int 
main(void) 
{
    test_str();
    test_buf();
    return 0;
}
