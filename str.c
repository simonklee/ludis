#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
 * What is a str and how should the interface look like.
 *
 * Should the be implemented like Go buffer or just a manipulation of
 * c char *. String functions should act on char *, while buffer should
 * act as a Go Buffer 
 *
 * the underlying type in a buffer is a dynamic char *. creating a buffer 
 * requires one call to malloc. 
 *
 * Principal functions used to manipulate a str
 *
 * len()   — returns the len of the buffer or the unread portion? O(1)
 * grow()  —  
 *
 *
 * */
typedef char * str;

struct str_header {
    int   len;
    int   alloc;
    char  buf[1]; /* s var length buf */
};

struct buffer {
    int off;
    str s;
};

/* str_new allocates a new buffer of n size.  it returns a pointer
 * to the str */
str
str_new(size_t n)
{
    struct str_header *b;

    b = calloc(1, offsetof(struct str_header, buf) + n);
    b->len = n;
    b->alloc = n;
    return b->buf;
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

    new_h = realloc(s, offsetof(struct str_header, buf) + h->alloc + n);

    if (new_h == NULL) 
        return NULL;

    new_h->alloc =+ n;
    return s;
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

/* str_read reads len(b) bytes from str.  returns n read or
 * EOF. */
int
buffer_read(struct buffer *b, char *p)
{
    int len = str_len(b->s);

    if (b->off >= len) {
        return EOF;
    }

    memcpy(p, b->s, len);
    b->off += len;
    return len;
}

char
buffer_read_byte(struct buffer *b)
{
    char c;

    if (b->off >= str_len(b->s)) {
        return EOF;
    }

    c = b->s[b->off];
    b->off++;

    return c;
}
/*
int 
buffer_read_from(struct buffer *b, int fd)
{
    int nread = 0;

    return nread;
}
*/
