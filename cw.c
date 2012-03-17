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
    const char *a = url;

    if (a[0] == 'h' || a[0] == 'H')
        a += 7; /* ignore http:// */

    if ((rv = handle_connect_gai(p->c, AF_UNSPEC, a, 80, &addr)) != LUDIS_OK)
        return rv;

    p->addr = addr;
    debug_netaddr(&p->addr);
    return LUDIS_OK;
}

static int
cw_fetch_url(struct page_handle *p, const char *url)
{
    int n;
    char buf[255];

    n = snprintf(buf, 255, "GET %s HTTP/1.1\r\nHost: simonklee.org\r\nConnection: Close\r\n\r\n", url);

    if (buffer_write(p->c->wb, buf, n) != n) {
        return LUDIS_ERR;
    }

    if (buffer_write_to(p->c->wb, p->c->fd) != n) {
        /* TODO: handle write errors */
        return LUDIS_ERR;
    }

    if ((n = buffer_read_from(p->c->rb, p->c->fd)) <= 0) {
        /* TODO: handle response err */
        return n;
    }

    /* printf("%s", p->c->rb->s);*/
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

    rv = cw_fetch_url(p, url);

    if (rv != LUDIS_OK)
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
