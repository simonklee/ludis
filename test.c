#include <malloc.h>

#include "test.h"

/* Returns a pointer to a char * which has escaped occurences of \r and \n */
static char *
escape_crlf(char *s)
{
    int c, newlen; 
    char *p;
    p = s;

    for(newlen=0; (c = *p++) != '\0'; newlen++)
        if (c == '\n' || c == '\r')
            newlen++;

    p = (char *)malloc(sizeof(char) * newlen + 1);

    while((c = *s++) != '\0') 
        switch(c) {
        case 10:
            *p++ = '\\';
            *p++ = 'n';
            break;
        case 13:
            *p++ = '\\';
            *p++ = 'r';
            break;
        default:
            *p++ = c;
            break;
        }

    *p = '\0';
    return p - newlen;
}

void
log_proto(char *s)
{
    char *p;
    p = escape_crlf(s);
    log_info("%s", p);
    free(p);
}
