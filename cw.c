#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "addr.h"
#include "common.h"
#include "handle.h"
#include "lmalloc.h"

struct page_handle {
    struct ludis_handle *c;
    struct net_addr addr;
    Str *url;
    Str *domain;
};

static Str *
url_domainname(Str *url)
{
    Str *domain;
    char *end, *start;
    int n;
    
    if (str_startswithcase(url, "http://", 7))
        start = url->data + 7;

    end = ludis_find(start, url->len - (url->data - start), "/", 1);

    if (end == NULL)
        end = url->data + url->len;

    n = end - start;
    domain = str_new(n);
    return str_append(domain, start, n);
}

static struct page_handle *
cw_page_handle_new(const char *url)
{
    struct page_handle *p;
    int n = strlen(url);

    p = lmalloc(sizeof(struct page_handle));
    p->c = handle_new();
    p->url = str_new(n);
    str_append(p->url, url, n);
    p->domain = url_domainname(p->url);

    return p;
}

static int
cw_page_handle_free(struct page_handle *p)
{
    handle_free(p->c);
    str_free(p->url);
    str_free(p->domain);
    free(p);
    return LUDIS_OK;
}

static int
cw_page_handle_connect(struct page_handle *p)
{
    int rv;
    struct net_addr addr;

    if ((rv = handle_connect_gai(p->c, AF_UNSPEC, p->domain->data, 80, &addr)) != LUDIS_OK)
        return rv;

    p->addr = addr;
    debug_netaddr(&p->addr);
    return LUDIS_OK;
}

static int
cw_fetch_url(struct page_handle *p)
{
    int n;
    char buf[255];

    n = snprintf(buf, 255, \
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: Close\r\n\r\n", p->url->data, p->domain->data);

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

    p = cw_page_handle_new(url);

    if ((rv = cw_page_handle_connect(p)) != LUDIS_OK)
        goto error;

    rv = cw_fetch_url(p);

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
