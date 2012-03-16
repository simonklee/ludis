/* Crawler is responsible for downloading HTML content from given urls */
#include <stdio.h>
#include <stdlib.h>

#include "addr.h"
#include "common.h"
#include "handle.h"
#include "lmalloc.h"

struct page_handle {
    struct ludis_handle *c;
    struct net_addr addr;
};

static struct page_handle *
cw_page_handle_new()
{
    struct page_handle *p;

    p = lmalloc(sizeof(struct page_handle));
    p->c = handle_new();
    return p;
}

static int
cw_page_handle_free(struct page_handle *p)
{
    handle_free(p->c);
    free(p);
    return LUDIS_OK;
}

static int
cw_page_handle_connect(struct page_handle *p, const char *url)
{
    int rv;
    struct net_addr addr;

    if ((rv = handle_connect_gai(p->c, AF_UNSPEC, url, 80, &addr)) != LUDIS_OK)
        return rv;

    p->addr = addr;
    debug_netaddr(&p->addr);
    return LUDIS_OK;
}

static int 
cw_get(const char *url) 
{
    int rv;
    struct page_handle *p;

    p = cw_page_handle_new();

    if ((rv = cw_page_handle_connect(p, url)) != LUDIS_OK)
        goto error;

    cw_page_handle_free(p);
    return LUDIS_OK;
error:
    cw_page_handle_free(p);
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
