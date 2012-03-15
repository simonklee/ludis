#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "lmalloc.h"
#include "str.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* str_new allocates a new Str buf of size n.  
 * Returns a pointer Str */
Str
str_new(size_t n)
{
    struct str_header *h;

    h = lcalloc(1, offsetof(struct str_header, buf) + n + 1);
    h->len = 0;
    h->cap = n;
    h->buf[n] = '\0';
    return h->buf;
}

/* str_len returns the current len of the Str buf */
int
str_len(const Str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->len;
}

/* str_avail returns the free space in the Str buf */
int
str_avail(const Str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->cap - h->len;
}

/* str_cap returns the total capacity space in the Str buf */
int
str_cap(const Str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    return h->cap;
}

/* str_grow guarantees that the Str buf has n free bytes 
 * returns a pointer to the newly Str */
Str
str_grow(Str s, const int n)
{
    struct str_header *h, *new_h;

    h = (void *)(s - offsetof(struct str_header, buf));

    if (h->cap - h->len >= n)
        return s;

    new_h = lrealloc(h, offsetof(struct str_header, buf) + h->cap + n + 1);
    new_h->cap =+ n;
    new_h->buf[new_h->cap] = '\0';
    return new_h->buf;
}

/* str_append appends n bytes from data to the end of Str 
 * returns a pointer to Str */
Str
str_append(Str s, const void *data, size_t n)
{
    struct str_header *h;
    Str p;

    p = str_grow(s, n);
    h = (void *)(p - offsetof(struct str_header, buf));
    memcpy(h->buf + h->len, data, n);
    h->len += n;
    return p;
}

/* str_appends appends strlen(data) bytes from data to the end of Str 
 * returns a pointer to Str */
Str
str_appends(Str s, const char *data)
{
    return str_append(s, data, strlen(data));
}

/* str_free free's Str */
void
str_free(Str s)
{
    struct str_header *h = (void *)(s - offsetof(struct str_header, buf));
    free(h);
}

/* str_truncate discards all but the first unread byte in the
 * Str */

/* str_reset discards the str.*/

/* buffer_new returns a new buffer */
Buffer *
buffer_new(size_t n) {
    Buffer *b;

    b = lmalloc(sizeof(Buffer));
    b->off = 0;
    b->s = str_new(n);
    return b;
}

/* buffer_new returns a new buffer for Str */
Buffer *
buffer_new_str(Str s) {
    Buffer *b;

    b = lmalloc(sizeof(Buffer));
    b->off = 0;
    b->s = s;
    return b;
}

/* buffer_free free's buffer */
void 
buffer_free(Buffer *b) 
{
    str_free(b->s);
    free(b);
}

/* buffer_len returns number of bytes in the unread portion of the buffer */
int 
buffer_len(Buffer *b)
{
    return str_len(b->s) - b->off;
}

/* buffer_next gives you a pointer to the next byte and advances read offset 
 * by n. returns n or the number of bytes availible if n is larger than buf len.
 * returns EOF if buffer is empty or drained. */
int
buffer_next(Buffer *b, char **p, int n)
{
    int len = buffer_len(b);

    if (len <= 0) {
        return EOF;
    }

    n = MIN(len, n);
    *p = b->s + b->off;
    b->off += n;
    return n;
}

/* reads the next n or until the buffer is drained.
 * returns n bytes read or EOF if no data is avail. */
int
buffer_read(Buffer *b, char *dest, int n)
{
    int len = buffer_len(b);

    if (len <= 0) {
        return EOF;
    }

    n = MIN(len, n);
    memcpy(dest, b->s + b->off, n);
    b->off += n;
    return n;
}

/* reads len(b) bytes from buffer. 
 * returns n read or EOF. */
int
buffer_reads(Buffer *b, char *p)
{
    return buffer_read(b, p, str_len(b->s));
}

/* buffer_write appends the contents of data to the buffer 
 * returns n write bytes */
int 
buffer_write(Buffer *b, const void *data, size_t n)
{
    b->s = str_append(b->s, data, n);
    return n;
}

/* buffer_write appends the contents of s to the buffer 
 * returns n write bytes */
int 
buffer_writes(Buffer *b, const char *s)
{
    return buffer_write(b, s, strlen(s));
}

/* buffer_read_byte read a single byte of the buffer
 * returns a single byte or EOF */
int
buffer_read_byte(Buffer *b)
{
    int c;

    if (buffer_len(b) <= 0) {
        return EOF;
    }

    c = b->s[b->off];
    b->off++;

    return c;
}

/* buffer_read_from reads from the file descriptor onto the buffer
 * returns n read bytes or LUDIS_ERR */
int 
buffer_read_from(Buffer *b, int fd)
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

/* buffer_write_to writes to the file descriptor until the buffer is drained
 * returns n write bytes or LUDIS_ERR */
int 
buffer_write_to(Buffer *b, int fd)
{
    assert(b);
    assert(fd);
    return LUDIS_ERR;
}
