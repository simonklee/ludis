#include <stdio.h>
#include <syscall.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define LUDIS_OK  0
#define LUDIS_ERR -1
#define READ_SIZE 16

struct query {
    char *buf;
    int   len;
    int   a;
};

unsigned char OK    = 1; 
unsigned char ERR   = 2; 
unsigned char INT   = 3; 
unsigned char BULK  = 4; 
unsigned char MULTI = 5; 

void bin(short d) {
    int i;
    for(i = 7; i >= 0; i--) {
        if((1 << i) & d)
            printf("1");
        else printf("0");
    }
}

void proto(short d) {
    bin(d);
    printf(" %X %d %c\n", d, d, d);
}

int 
call(char *bytes)
{
    switch(bytes[0]) {
    case '+':
        return OK;
    case '-':
        return ERR;
    case ':':
        return INT;
    case '$':
        return BULK;
    case '*':
        return MULTI;
    }

    return LUDIS_ERR;
}    

struct query *
new_query(void)
{
    struct query *q;
    
    q = (struct query *) malloc(sizeof(struct query)); 
    assert(q);
    q->buf = (char *)malloc(sizeof(char) * READ_SIZE);
    assert(q->buf);
    q->len = 0;
    q->a = READ_SIZE;

    return q;
}

void
free_query(struct query *q)
{
    free(q->buf);
    free(q);
}

int
write_query(struct query *q, const char *filename) 
{
    FILE *fp = NULL;
    short nwrite;

    fp = fopen(filename, "w+");

    if (!fp) {
        fprintf(stderr, "fopen(%s): %s\n", filename, strerror(errno));
        goto error;
    }

    nwrite = fwrite(q->buf, sizeof(char), q->len, fp);

    if (ferror(fp)) {
        fprintf(stderr, "fwrite(%s): %s\n", filename, strerror(errno));
        goto error;
    }

    fprintf(stdout, "%d bytes written\n", nwrite);
    fclose(fp);
    return LUDIS_OK;
error:
    if (fp) fclose(fp);
    return LUDIS_ERR;
}

int
read_query(struct query *q, const char *filename) 
{
    FILE *fp = NULL;
    char buf[READ_SIZE];
    short nread;

    fp = fopen(filename, "r");

    if (!fp) {
        fprintf(stderr, "fopen(%s): %s\n", filename, strerror(errno));
        goto error;
    }

    nread = fread(buf, sizeof(char), sizeof(buf), fp);

    if (ferror(fp)) {
        fprintf(stderr, "fread(%s): %s\n", filename, strerror(errno));
        goto error;
    }

    fprintf(stdout, "%d bytes read\n", nread);

    if (nread) {
        memcpy(q->buf, buf, nread);
        q->len = nread;
    } else {
        goto error;
    }

    fclose(fp);
    return LUDIS_OK;
error:
    if (fp) fclose(fp);
    return LUDIS_ERR;
}

#ifdef TEST
#include "test.h"
int main(void) 
{
    struct query *q;
    int i;
    q = new_query();
    /*q->buf[0] = '+';
    q->buf[1] = 'O';
    q->buf[2] = 'K';
    q->buf[3] = '\r';
    q->buf[4] = '\n';
    q->len = 5;

    write_query(q, "query.rp");*/
    if (read_query(q, "query.rp") != LUDIS_OK) {
        fprintf(stderr, "err\n");
        goto error;
    }
    
    for (i = 0; i < q->len; i++) {
        printf("%X\n", q->buf[i]);
    }
    
    free_query(q);
    return 0;
error:
    if (q) free_query(q);
    return 1;
}
#endif
