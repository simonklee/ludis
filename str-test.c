#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "common.h"
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
    Buffer *b;
    char p[16];
    char *ptr;
    int i;

    b = buffer_new(0);

    assert(buffer_len(b)== 0);
    assert(buffer_read(b, p, 16)== LUDIS_EEOF);
    assert(buffer_reads(b, p) == LUDIS_EEOF);

    assert(buffer_writes(b, "hello") == 5);
    assert(buffer_len(b)== 5);
    assert(buffer_read(b, p, 2) == 2); 
    assert(buffer_reads(b, p+2) == 3);
    assert(buffer_reads(b, p+5) == LUDIS_EEOF); /* drained */

    for (i = 0; i < 5; i++) {
        /* printf("%c vs %c\n", b->s->data[i], p[i]); */
        assert(b->s->data[i] == p[i]);
    }

    buffer_free(b);

    b = buffer_new(0);
    assert(buffer_writes(b, "hello") == 5);
    assert(buffer_next(b, &ptr, 1));
    assert(ptr[0] == 'h');
    /*assert(ptr[6] == '\0');*/
}

TEST(strstrn) {
    Str *s = str_new(16);
    char *p;

    str_append(s, "foo\0bar", 7);
    /* strncmp is not binary safe. */
    assert(strncmp(s->data, "foo\0baz", 7) == 0); 
    assert(memcmp(s->data, "foo\0baz", 7) != 0); 

    p = str_find(s, "oo", 2);
    printf("starts at: %c\n", *p);
    assert(p == s->data + 1);
    str_free(s);
}

TEST(strcasestrn) {
    Str *s;

    s = str_new(6);
    str_appends(s, "abcdef");

    assert(str_findcase(s, "cde", 3) == s->data + 2);
    assert(str_findcase(s, "abcdef", 6) == s->data);
    assert(str_findcase(s, "a", 1) == s->data);
    assert(str_findcase(s, "f", 1) == s->data + 5);

    assert(str_findcase(s, "abcdefg", 7) == NULL); 
    assert(str_findcase(s, "fg", 2) == NULL);
    assert(str_findcase(s, "", 0) == NULL);
    assert(str_findcase(s, "aabcde", 6) == NULL);

    assert(str_findcase(s, "Cde", 3) == s->data + 2);
    assert(str_findcase(s, "aBCdef", 6) == s->data);
    assert(str_findcase(s, "A", 1) == s->data);
    assert(str_findcase(s, "F", 1) == s->data + 5);

    str_free(s);
}

TEST(startswith) {
    Str *s;

    s = str_new(6);
    str_appends(s, "abcdef");

    assert(str_startswith(s, "cde", 3) == 0);
    assert(str_startswith(s, "abcdef", 6) == 1);
    assert(str_startswith(s, "def", 3) == 0);
    assert(str_startswith(s, "a", 1) == 1);
 
    str_free(s);
}

TEST(startswithcase) {
    Str *s;

    s = str_new(6);
    str_appends(s, "ABcDef");

    assert(str_startswithcase(s, "Cde", 3) == 0);
    assert(str_startswithcase(s, "aBcDeF", 6) == 1);
    assert(str_startswithcase(s, "DEf", 3) == 0);
    assert(str_startswithcase(s, "A", 1) == 1);
 
    str_free(s);
}

TEST(str) {
    Str *s;
    char p[16];

    s = str_new(16);

    assert(str_len(s) == 0);
    assert(str_avail(s) == 16);

    sprintf(p, "hello");
    s = str_appends(s, p);

    assert(str_len(s) == (int)strlen(p));
    assert(str_avail(s) == str_cap(s) - (int)strlen(p));

    str_free(s);
}

int 
main(void) 
{
    test_str();
    test_buf();
    test_strstrn();
    test_strcasestrn();
    test_startswith();
    test_startswithcase();
    return 0;
}
