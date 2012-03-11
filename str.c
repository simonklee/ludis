#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "str.h"
#include "common.h"
#include "lmalloc.h"

/* str_new allocates a new str buf of size n.  
 * Returns a pointer str */
str
str_new(size_t n)
{
    struct str_header *h;

    h = lcalloc(1, offsetof(struct str_header, buf) + n + 1);
    h->len = 0;
    h->alloc = n;
    h->buf[n] = '\0';
    return h->buf;
}

/* str_len returns the current len of the str buf */
int
str_len(const str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->len;
}

/* str_avail returns the free space in the str buf */
int
str_avail(const str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->alloc - h->len;
}

/* str_grow guarantees that the str buf has n free bytes 
 * returns a pointer to the newly str */
str
str_grow(str s, const int n)
{
    struct str_header *h, *new_h;

    h = (void *)(s - offsetof(struct str_header, buf));

    if (h->alloc - h->len >= n)
        return s;

    new_h = lrealloc(h, offsetof(struct str_header, buf) + h->alloc + n + 1);
    new_h->alloc =+ n;
    new_h->buf[new_h->alloc] = '\0';
    return new_h->buf;
}

/* str_append appends n bytes from data to the end of str 
 * returns a pointer to str */
str
str_append(str s, const void *data, size_t n)
{
    struct str_header *h;
    str p;

    p = str_grow(s, n);
    h = (void *)(p - offsetof(struct str_header, buf));
    memcpy(h->buf + h->len, data, n);
    h->len += n;
    return p;
}

/* str_appends appends strlen(data) bytes from data to the end of str 
 * returns a pointer to str */
str
str_appends(str s, const char *data)
{
    return str_append(s, data, strlen(data));
}

/* str_free free's str */
void
str_free(str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    free(h);
}

/* str_truncate discards all but the first unread byte in the
 * str */

/* str_reset discards the str.*/

/* buffer_new returns a new buffer for str */
struct buffer *
buffer_new(str s) {
    struct buffer *b;

    b = lmalloc(sizeof(struct buffer));
    b->off = 0;
    b->s = s;
    return b;
}

/* buffer_free free's buffer */
void 
buffer_free(struct buffer *b) 
{
    free(b);
}

/* reads len(b) bytes from buffer. 
 * returns n read or if n > b->off. */
int
buffer_read(struct buffer *b, char *p, int n)
{
    if (b->off >= n) {
        return EOF;
    }

    memcpy(p, b->s, n);
    b->off += n;
    return n;
}

/* reads len(b) bytes from buffer. 
 * returns n read or EOF. */
int
buffer_reads(struct buffer *b, char *p)
{
    return buffer_read(b, p, str_len(b->s));
}

/* buffer_read_byte read a single byte of the buffer
 * returns a single byte or EOF */
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

/* buffer_read_from reads from the file descriptor onto the buffer
 * returns nread */
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
