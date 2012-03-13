#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "lmalloc.h"
#include "ludis.h"
#include "str.h"

/* we do not know how many arguments we recieved
 * we can only know by parsing the fmt str and 
 * counting % occurences.
 *
 * accept two formats:
 *
 *     %s - \0 terminated string
 *     %b - binary blob of data, requires size_t
 *
 * parse str
 *
 *     1. check if byte is '%'
 *         YES. 
 *             check if we are at the end of a cmd
 *                 YES.
 *                     add cmd to commands list
 *             continue to %s and %b parser
 *         NO.
 *             check if byte is ' '
 *                YES.
 *                    end of a cmd
 *                NO.
 *                
 * */
int 
vformat(char *str, const char *fmt, va_list ap)
{
    const char *fmt_pos = fmt;
    int argc, off, c, i;
    char *b;
    size_t len;
    char **argv;
    Str s;

    argv = NULL;
    argc = off = 0;

    while((c = *fmt_pos++) != '\0') {
        printf("--> %c ", c);
        switch(c) {
        case '%':
            off += 2;
            c = *fmt_pos++;
            printf("\n");

            switch(c) {
            case 'b':
                b = va_arg(ap, char *);
                len = va_arg(ap, size_t);
                break;
            case 's':
                b = va_arg(ap, char *);
                len = strlen(b);
                break;
            default:
                goto error;
            }

            s = str_new(len);
            s = str_append(s, b, len);
            break;
        case ' ':
            printf("space ... ");
            len = (fmt_pos - (fmt + off)) - 1; 

            if (len > 0) {
                printf("appending ... ");
                s = str_new(len);
                s = str_append(s, fmt + off, len);
                printf("%d ... ", len);
                off += len;
                printf("%s", s);
            } else {
                printf("NOT appending");
                /* There is nothing to append */
                /*        |     */
                /* "SET %s bar" */
            }

            off++;
            printf("\n");
            break;
        default:
            printf("\n");
            continue;
        }
        
        argv = lrealloc(argv, sizeof(Str) * (argc + 1));
        argv[argc++] = s;
    }

    printf("%d\n", argc);

    for (i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return argc;
error:
    return LUDIS_ERR;
}

int
format(char *str, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vformat(str, fmt, ap);
    va_end(ap);

    return ret;
}

/* parse a fmt str */
int 
call(const char *fmt, ...)
{
    assert(fmt != NULL);
    return LUDIS_OK;
}
