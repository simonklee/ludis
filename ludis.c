#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "common.h"
#include "lmalloc.h"
#include "test.h"
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

struct command {
    int  len;
    int  argc;
    Str *argv;
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
    /*     $   intlen  \r\n   data        \r\n */
    return 1 + int_len(bulklen) + 2 + bulklen + 2;
}

/* build a new command by concencate an array 
*  which contains the arguments for the command 
*  returns the len of the command */
int 
format_argv(char **dest, struct command *cmd)
{
    int i, size;
    char buf[16];
    Str out;

    cmd->len += 1 + int_len(cmd->argc) + 2;
    out = str_new(cmd->len);
    size = sprintf(buf, "*%d\r\n", cmd->argc);
    out = str_append(out, buf, size);

    for (i = 0; i < cmd->argc; i++) {
        size = sprintf(buf, "$%d\r\n", cmd->argc);
        out = str_append(out, buf, size);
        out = str_append(out, cmd->argv[i], str_len(cmd->argv[i]));
        out = str_append(out, "\r\n", 2);
        str_free(cmd->argv[i]);
    }

    lfree(cmd->argv);
    assert(str_len(out) == cmd->len);
    *dest = out;

    /* debug */
    log_proto(out);
    printf("len:\t%d\n", cmd->len);
    printf("argc:\t%d\n", cmd->argc);

    return cmd->len;
}

int 
vformat(char **str, const char *fmt, va_list ap)
{
    int off, c, size;
    const char *p = fmt;
    char *item;
    Str s;
    struct command cmd;

    cmd.argv = NULL;
    cmd.argc = 0;
    cmd.len = 0;
    off = 0;

    while((c = *p++) != '\0') {
        /* debug */
        printf("--> %c ", c);

        switch(c) {
        case '%':
            off += 2;
            c = *p++;

            switch(c) {
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
            printf("found %c ... ", c);
            printf("%d ... ", size);
            printf("%s ... ", s);

            break;

        case ' ':
            size = (p - (fmt + off)) - 1; 

            if (size > 0) {
                s = str_new(size);
                s = str_append(s, fmt + off, size);
                off += size;

                /* debug */
                printf("appending ... ");
                printf("%d ... ", size);
                printf("%s ... ", s);
            } else {
                printf("NOT appending");
                /*        |     */
                /* "SET %s bar" */
            }

            off++;
            break;
            
        default:
            if (p[1] == '\0') {
                p++;
                size = (p - (fmt + off)) - 1; 
                printf("size: %d, off: %d\n", size, off);

                if (size > 0) {
                    s = str_new(size);
                    s = str_append(s, fmt + off, size);
                    off += size;
                }
                off++;
                break;
            } else {
                /* debug */
                printf("\n");
                continue;
            }
        }
        
        cmd.argv = lrealloc(cmd.argv, sizeof(Str) * (cmd.argc + 1));
        cmd.argv[cmd.argc++] = s;
        cmd.len += arg_len(size);

        /* debug */
        printf("%d\n", 1 + 1     + 2    + str_len(s) + 2);
    }

    return format_argv(str, &cmd);
error:
    while(cmd.argc--)
        str_free(cmd.argv[cmd.argc]);

    if (cmd.argv != NULL)
        lfree(cmd.argv);

    return LUDIS_ERR;
}

int
format(char **str, const char *fmt, ...)
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
