#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "http.h"

static int 
cw_get(const char *url) 
{
    int rv;
    struct http_response *r;

    r = http_init();
    rv = http_get(r, url); 

    if (rv != LUDIS_OK)
        goto error;

    http_free(r);
    return LUDIS_OK;
error:
    http_free(r);
    return LUDIS_ERR;
}

int
main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stdout, "usage: %s url [url ...]\n", argv[0]);
        exit(1);
    }

    while (--argc > 0) {
        printf("  GET %s", argv[argc]);

        if (cw_get(argv[argc]) == LUDIS_OK) {
            printf(" ... OK\n");
        } else  {
            printf(" ... ERR\n");
        }
    }

    return 0;
}
