#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "str.h"
#include "common.h"
#include "lmalloc.h"

/* str_new allocates a new buffer of n size.  it returns a pointer
 * to the str */
str
str_new(size_t n)
{
    struct str_header *h;

    h = lcalloc(1, offsetof(struct str_header, buf) + n);
    h->len = 0;
    h->alloc = n;
    return h->buf;
}

int
str_len(const str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->len;
}

int
str_avail(const str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->alloc - h->len;
}

str
str_grow(str s, const int n)
{
    struct str_header *h, *new_h;

    h = (void *)(s - offsetof(struct str_header, buf));

    if (h->alloc - h->len >= n)
        return s;

    new_h = lrealloc(s, offsetof(struct str_header, buf) + h->alloc + n);
    new_h->alloc =+ n;
    return s;
}

str
str_append(str s, const void *data, size_t m)
{
    struct str_header *h;
    str p;

    p = str_grow(s, m);
    h = (void *)(p - offsetof(struct str_header, buf));
    memcpy(h->buf + h->len, data, m);
    h->len += m;
    return p;
}

str
str_appends(str s, const char *data)
{
    return str_append(s, data, strlen(data));
}

void
str_free(str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    free(h);
}

/* str_truncate discards all but the first unread byte in the
 * str */

/* str_reset discards the str.*/

struct buffer *
buffer_new(str s) {
    struct buffer *b;
    b->off = 0;
    b->s = s;
    return b;
}

/* str_read reads len(b) bytes from buffer. 
 * returns n read or if n > b->off. */
int
buffer_nread(struct buffer *b, char *p, int n)
{
    if (b->off >= n) {
        return EOF;
    }

    memcpy(p, b->s, n);
    b->off += n;
    return n;
}

/* str_read reads len(b) bytes from buffer. 
 * returns n read or EOF. */
int
buffer_read(struct buffer *b, char *p)
{
    return buffer_nread(b, p, str_len(b->s));
}

char
buffer_read_byte(struct buffer *b)
{
    int c;

    if (b->off >= str_len(b->s)) {
        return EOF;
    }

    c = b->s[b->off];
    b->off++;

    return c;
}

int 
buffer_read_from(struct buffer *b, int fd)
{
    int nread = 0;
    char *p;

    p = str_grow(b->s, IOBUFLEN);
    nread = read(fd, p + str_len(p), IOBUFLEN);

    switch(nread) {
    case -1:
        /* check errno IO errors */
        /*if (errno == EAGAIN) {
            async 
        } else {
            goto error;
        }*/
        goto error;
    case 0:
        /* EOF/connection lost */
        goto error;
    } 

    b->s = p;
    return nread;
error:
    close(fd);
    return LUDIS_ERR;
}
