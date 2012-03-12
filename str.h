#ifndef _STR_H
#define _STR_H

/* str implements a binary safe dynamic string library */

typedef char * str;

struct str_header {
    int   cap;    /* nr of allocated bytes */
    int   len;    /* nr of used bytes */
    char  buf[1]; /* s var length buf */
};

int str_avail(const str s);
int str_cap(const str s);
int str_len(const str s);
str str_append(str s, const void *data, size_t n);
str str_appends(str s, const char *data);
str str_grow(str s, const int n);
str str_new(size_t n);
void str_free(str s);

/* buffer implements a simple variable sized byte buffer
 * on top of the str interface.*/

struct buffer {
    int off;      /* current read offset */
    str s;
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
struct buffer *buffer_new_str(str s);
void buffer_free(struct buffer *b);

#endif
