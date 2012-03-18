#ifndef LUDIS_STR_H
#define LUDIS_STR_H

/* generic string functions used by str library */

char *ludis_find(const char *s, int n, const char *needle, int needlelen);
char *ludis_findcase(const char *s, int n, const char *needle, int needlelen);

/* Str implements a binary safe dynamic string library */

typedef struct str_header Str;

struct str_header {
    int   cap;     /* nr of allocated bytes */
    int   len;     /* nr of used bytes */
    char  data[1]; /* s var length buf */
};

int str_avail(Str *s);
int str_cap(Str *s);
int str_len(Str *s);
Str *str_append(Str *s, const void *data, size_t n);
Str *str_appends(Str *s, const char *data);
Str *str_cat(Str *s, Str *b);
char *str_find(Str *s, const char *needle, int n);
char *str_findcase(Str *s, const char *needle, int n);
int str_startswith(Str *s, const char *needle, int n);
int str_startswithcase(Str *s, const char *needle, int n);
Str *str_grow(Str *s, const int n);
Str *str_new(size_t n);
void str_free(Str *s);

/* buffer implements a simple variable sized byte buffer
 * on top of the Str interface.*/

typedef struct str_buffer Buffer;

struct str_buffer {
    int off;      /* current read offset */
    Str *s;
};

int buffer_len(Buffer *b);
int buffer_next(Buffer *b, char **p, int n);
int buffer_read(Buffer *b, char *dest, int n);
int buffer_read_byte(Buffer *b);
int buffer_read_from(Buffer *b, int fd);
int buffer_reads(Buffer *b, char *p);
int buffer_write(Buffer *b, const void *data, size_t n);
int buffer_write_to(Buffer *b, int fd);
int buffer_writes(Buffer *b, const char *s);
Buffer *buffer_new(size_t n);
Buffer *buffer_new_str(Str *s);
void buffer_free(Buffer *b);

#endif
