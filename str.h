#ifndef _STR_H
#define _STR_H

/* Str implements a binary safe dynamic string library */

typedef char *Str;

struct str_header {
    int   cap;    /* nr of allocated bytes */
    int   len;    /* nr of used bytes */
    char  buf[1]; /* s var length buf */
};

int str_avail(const Str s);
int str_cap(const Str s);
int str_len(const Str s);
Str str_append(Str s, const void *data, size_t n);
Str str_appends(Str s, const char *data);
Str str_grow(Str s, const int n);
Str str_new(size_t n);
void str_free(Str s);

/* buffer implements a simple variable sized byte buffer
 * on top of the Str interface.*/

struct buffer {
    int off;      /* current read offset */
    Str s;
};

int buffer_len(struct buffer *b);
int buffer_next(struct buffer *b, char **p, int n);
int buffer_read(struct buffer *b, char *dest, int n);
int buffer_read_byte(struct buffer *b);
int buffer_read_from(struct buffer *b, int fd);
int buffer_reads(struct buffer *b, char *p);
int buffer_write(struct buffer *b, const void *data, size_t n);
int buffer_write_to(struct buffer *b, int fd);
int buffer_writes(struct buffer *b, const char *s);
struct buffer *buffer_new(size_t n);
struct buffer *buffer_new_str(Str s);
void buffer_free(struct buffer *b);

#endif
