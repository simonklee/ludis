#ifndef _STR_H
#define _STR_H

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
    int   alloc;  /* nr of allocated bytes */
    int   len;    /* nr of used bytes */
    char  buf[1]; /* s var length buf */
};

struct buffer {
    int off;      /* current read offset */
    str s;
};

str str_new(size_t n);
int str_len(const str s);
int str_avail(const str s);
str str_grow(str s, const int n);
str str_append(str s, const void *data, size_t m);
str str_appends(str s, const char *data);
void str_free(str s);

struct buffer *buffer_new(str s);
int buffer_read(struct buffer *b, char *p);
int buffer_nread(struct buffer *b, char *p, int n);
char buffer_read_byte(struct buffer *b);
int buffer_read_from(struct buffer *b, int fd);

#endif
