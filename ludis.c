#include <stdio.h>

#define STATUS_OK  0
#define STATUS_ERR 1

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
    printf("\n");
}

int
read_query(int fd, char *query) 
{
    char buf[16];
    short nread;

    nread = read(fd, buf, 16);

    return STATUS_OK;
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

    return STATUS_ERR;
}    

#ifdef TEST
#include "test.h"
int main(void) 
{
    unsigned char types = 0;
    types |= OK;
    bin(types);

    types |= ERR;
    bin(types);

    types |= (INT | BULK | MULTI);
    bin(types);

    types &= ~(INT | BULK | MULTI);
    bin(types);
    
    return 0;
}
#endif
