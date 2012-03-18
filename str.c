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
#include "fd.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ludis_tolower(c) (((c)>='A' && (c) <='Z') ? c|0x20 : c)

/*static char
ludis_tolower(char c)
{
    return (c >= 'A' && c <= 'Z') ? c|0x20 : c;
}*/

char *
ludis_find(const char *s, int n, const char *needle, int needlelen)
{
    char c1, c2, *haystack = (char *)s;
    int len = n;

    c2 = *needle++;

    do {
        do {
            if (len-- == 0)
                return NULL;

            c1 = *haystack++;

        } while (c1 != c2);

    } while (memcmp(haystack, needle, needlelen) != 0);

    return --haystack;
}

char *
ludis_findcase(const char *s, int n, const char *needle, int needlelen)
{
    char *a, *b;
    int al, bl, i;

    for (i = 0; i + needlelen <= n; i++) {
        a = (char *) s + i;
        al = n - i;

        b = (char *) needle;
        bl = needlelen;

        while (ludis_tolower(*a) == ludis_tolower(*b)) {
            a++; b++; al--;

            if (--bl == 0) /* end of needle, success */
                return (char *) s + i;
        }
    }

    return NULL;
}

/* str_new allocates a new Str buf of size n.  
 * Returns a pointer Str */
Str *
str_new(size_t n)
{
    Str *s;

    s = lcalloc(1, offsetof(Str, data) + n + 1);
    s->len = 0;
    s->cap = n;
    s->data[n] = '\0';
    return s;
}

/* str_len returns the current len of the Str buf */
int
str_len(Str *s)
{
    return s->len;
}

/* str_avail returns the free space in the Str buf */
int
str_avail(Str *s)
{
    return s->cap - s->len;
}

/* str_cap returns the total capacity space in the Str buf */
int
str_cap(Str *s)
{
    return s->cap;
}

/* str_grow guarantees that the Str buf has n free bytes 
 * returns a pointer to the new Str */
Str *
str_grow(Str *s, const int n)
{
    Str *new_s;

    if (str_avail(s) >= n)
        return s;

    new_s = lrealloc(s, offsetof(Str, data) + s->cap + n + 1);
    new_s->cap =+ n;
    new_s->data[new_s->cap] = '\0';
    return new_s;
}

/* str_append appends n bytes from data to the end of Str 
 * returns a pointer to Str */
Str *
str_append(Str *s, const void *data, size_t n)
{
    assert(n > 0);

    s = str_grow(s, n);
    memcpy(s->data + s->len, data, n);
    s->len += n;
    return s;
}

/* str_appends appends strlen(data) bytes from data to the end of Str 
 * returns a pointer to Str */
Str *
str_appends(Str *s, const char *data)
{
    return str_append(s, data, strlen(data));
}

/* Concates b->data to s */
Str *
str_cat(Str *s, Str *b)
{
    return str_append(s, b->data, b->len);
}

/* str_free free's Str */
void
str_free(Str *s)
{
    free(s);
}

/* str_find finds the first occurance of the substring 
 * needle in the Str s.
 * returns a pointer to the beginning of substr or NULL */
char *
str_find(Str *s, const char *needle, int n) 
{
    return ludis_find(s->data, s->len, needle, n);
}

/* str_findcase finds the first occurance of the substring 
 * needle in the Str s. Ignores the case of chars.
 * returns a pointer to the beginning of substr or NULL */
char *
str_findcase(Str *s, const char *needle, int n)
{
    return ludis_findcase(s->data, s->len, needle, n);
}

/* str_startswith checks if Str s starts with a 
 * sequence of bytes.
 * returns 1 on success or 0 */
int
str_startswith(Str *s, const char *needle, int n)
{
    if (n > s->len)
        return 0;

    while (n--)
        if (s->data[n] != needle[n])
            return 0;

    return 1;
}

/* str_startswithcase checks if Str s starts with a 
 * sequence of bytes. Ignores the case of chars.
 * returns 1 on success or 0 */
int
str_startswithcase(Str *s, const char *needle, int n)
{
    if (n > s->len)
        return 0;

    while (n--)
        if (ludis_tolower(s->data[n]) != ludis_tolower(needle[n]))
            return 0;

    return 1;
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

/* buffer_new_str returns a new buffer for Str */
Buffer *
buffer_new_str(Str *s) {
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
 * returns LUDIS_EEOF if buffer is empty or drained. */
int
buffer_next(Buffer *b, char **p, int n)
{
    int len = buffer_len(b);

    if (len <= 0) {
        return LUDIS_EEOF;
    }

    n = MIN(len, n);
    *p = b->s->data + b->off;
    b->off += n;
    return n;
}

/* reads the next n or until the buffer is drained.
 * returns n bytes read or LUDIS_EEOF if no data is avail. */
int
buffer_read(Buffer *b, char *dest, int n)
{
    int len = buffer_len(b);

    if (len <= 0) {
        return LUDIS_EEOF;
    }

    n = MIN(len, n);
    memcpy(dest, b->s->data + b->off, n);
    b->off += n;
    return n;
}

/* reads len(b) bytes from buffer. 
 * returns n read or LUDIS_EEOF. */
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
 * returns a single byte or LUDIS_EEOF */
int
buffer_read_byte(Buffer *b)
{
    int c;

    if (buffer_len(b) <= 0) {
        return LUDIS_EEOF;
    }

    c = b->s->data[b->off];
    b->off++;

    return c;
}

/* buffer_read_from reads from fd to buffer *b until EOF
 * returns n read bytes or LUDIS_ESYS */
int 
buffer_read_from(Buffer *b, int fd)
{
    int n, nread = 0;
    char buf[IOBUFLEN];

    /* TODO: Truncate buffer first, if empty */
    do {
        n = fd_read(fd, buf, IOBUFLEN);

        if (n > 0) {
            buffer_write(b, buf, n);
            nread += n;
        }
    } while(n > 0);

    if (n == LUDIS_ESYS && errno != EAGAIN)
        return n;

    return nread;
}

/* buffer_write_to writes to fd until the buffer is drained
 * returns n write bytes or LUDIS_ESYS */
int 
buffer_write_to(Buffer *b, int fd)
{
    int rv, n, nwrite = 0;

    while ((n = buffer_len(b)) > 0) {
        n = MIN(IOBUFLEN, n);
        rv = fd_write(fd, b->s->data + b->off, n);

        if (rv == LUDIS_ESYS)
            return rv;

        assert(rv == n);
        b->off += n;
        nwrite += n;
    }

    /* TODO: Truncate buffer */
    return nwrite;
}
