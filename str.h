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
    int   len;
    int   alloc;
    char  buf[1]; /* s var length buf */
};

struct buffer {
    int off;
    str s;
};

str str_new(size_t n);
int str_len(const str s);
int str_avail(const str s);
str str_grow(str s, const int n);
void str_free(str s);

int buffer_read(struct buffer *b, char *p);
char buffer_read_byte(struct buffer *b);

#endif
