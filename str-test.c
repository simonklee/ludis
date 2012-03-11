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
    str s;
    struct buffer *b;
    char p[16];
    int m, nread, i;

    s = str_new(16);
    assert((m = str_len(s)) == 0);
    printf("%d\n", m);
    printf("%s\n", s);
    str_appends(s, "hello");

    m = str_len(s);
    b = buffer_new(s);
    nread = buffer_reads(b, p);

    printf("%d == %d\n", m, nread);
    assert(nread == m);

    for (i = 0; i < nread; i++)
        printf("%c", b->s[i]);
        /* assert(b->s[i] == s[i]); */

    printf("\n");

    str_free(s);
    buffer_free(b);
}

int 
main(void) 
{
    test_str();
    return 0;
}
