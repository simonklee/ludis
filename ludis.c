#include <stdio.h>

#define LUDIS_OK  0
#define LUDIS_ERR -1

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
read_query(int fd, char *query) 
{
    char buf[16];
    short nread;

    nread = read(fd, buf, 16);

    return LUDIS_OK;
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

#ifdef TEST
#include "test.h"
int main(void) 
{
    proto('+');
    proto('-');
    proto(':');
    proto('$');
    proto('*');
    proto('\n');
    proto('\r');
    proto('3');
/*    types |= OK;
    bin(types);

    types |= ERR;
    bin(types);

    types |= (INT | BULK | MULTI);
    bin(types);

    types &= ~(INT | BULK | MULTI);
    bin(types);*/
    
    return 0;
}
#endif
