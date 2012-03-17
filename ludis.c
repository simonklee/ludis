#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "common.h"
#include "lmalloc.h"
#include "test.h"
#include "ludis.h"
#include "str.h"

struct command {
    int  len;
    int  argc;
    Str **argv;
};

/* calculates the len of an int */
int
int_len(int n)
{
    int l = 0;

    if (n < 0) {
        n = -n;
        l++;
    }

    do {
        l++;
    } while(n /= 10);

    return l;
}

/* returns the len of an argument */
static size_t 
arg_len(size_t bulklen)
{
    /*     $   int_len           \r\n data     \r\n */
    return 1 + int_len(bulklen) + 2 + bulklen + 2;
}

/* build a new command by concencate an array 
*  which contains the arguments for the command 
*  returns the len of the command */
int 
format_argv(Str **dest, struct command *cmd)
{
    int i, size;
    char buf[16];
    Str *out;

    cmd->len += 1 + int_len(cmd->argc) + 2;
    out = str_new(cmd->len);
    size = sprintf(buf, "*%d\r\n", cmd->argc);
    out = str_append(out, buf, size);

    /* debug */
    /* printf("len:\t%d\n", cmd->len);
    printf("argc:\t%d\n", cmd->argc);
    printf("data:\t%s\n", out->data); */

    for (i = 0; i < cmd->argc; i++) {
        size = sprintf(buf, "$%d\r\n", cmd->argv[i]->len);
        out = str_append(out, buf, size);
        out = str_cat(out, cmd->argv[i]);
        out = str_append(out, "\r\n", 2);
        str_free(cmd->argv[i]);
    }

    lfree(cmd->argv);
    assert(str_len(out) == cmd->len);
    *dest = out;

    /* debug */
    /*log_proto(out->data);*/
    
    return cmd->len;
}

int 
vformat(Str **str, const char *fmt, va_list ap)
{
    int off, size;
    const char *p = fmt;
    char *item;
    Str *s;
    struct command cmd;

    cmd.argv = NULL;
    cmd.argc = 0;
    cmd.len = 0;
    off = 0;

    /* debug
    for(i = 0; p[i] != '\0'; i++) {
        printf("fmt + %d: %p - `%c`\n", i, &fmt[i], fmt[i]);
    } */

    for(; *p != '\0'; p++) {
        /* debug */
        /* printf("--> %c %p ", *p, p); */

        if (*p == '%') {
            off += 2;

            switch(*++p) {
            case 'b':
                item = va_arg(ap, char *);
                size = va_arg(ap, size_t);
                break;

            case 's':
                item = va_arg(ap, char *);
                size = strlen(item);
                break;

            default:
                goto error;
            }

            s = str_new(size);
            s = str_append(s, item, size);

            /* debug */
            /* printf("found %c ... ", *p);
            printf("%d ... ", size);
            printf("%s ... ", s->data); */
        } else {
            if (*p == ' ') {
                size = (p - (fmt + off)); 
            } else if (*(p + 1) == '\0') {
                size = (p + 1 - (fmt + off)); 
            } else {
                /* printf("\n"); */
                continue;
            }

            if (size > 0) {
                s = str_new(size);
                s = str_append(s, fmt + off, size);
                off += size + 1;

                /* debug */
                /* printf("appending ... ");
                printf("%d ... ", size);
                printf("%s ... ", s->data); */
            } else {
                /* printf("NOT appending "); */
                /*        |     */
                /* "SET %s bar" */
                off++;
                continue;
            }
        }
        
        cmd.argv = lrealloc(cmd.argv, sizeof(Str) * (cmd.argc + 1));
        cmd.argv[cmd.argc++] = s;
        cmd.len += arg_len(size);

        /* debug */
        /* printf("%d\n", 1 + 1     + 2    + s->len + 2); */
    }

    return format_argv(str, &cmd);
error:
    while(cmd.argc--)
        str_free(cmd.argv[cmd.argc]);

    if (cmd.argv != NULL)
        lfree(cmd.argv);

    return LUDIS_ERR;
}

/* we do not know how many arguments we recieved
 * accept two formats:
 *
 *     %s - \0 terminated string
 *     %b - binary blob of data, requires size_t
 * */
int
format(Str **str, const char *fmt, ...)
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
