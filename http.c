#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deps/http_parser/http_parser.h"
#include "addr.h"
#include "common.h"
#include "lmalloc.h"
#include "http.h"
#include "str.h"

static int http_connect(struct http_response *r);
static Str *url_domainname(Str *url);

struct http_response *
http_init() 
{
    struct http_response *r;

    r = lmalloc(sizeof(struct http_response));
    r->handle = handle_new();
    r->url = NULL;
    r->body = NULL;

    return r;
}

int 
http_free(struct http_response *r)
{
    handle_free(r->handle);

    if (r->url) str_free(r->url);
    if (r->body) str_free(r->body);

    free(r);
    return LUDIS_OK;
}

int 
http_get(struct http_response *r, const char *url)
{
    int rv;
   
    r->url = str_new_str(url, strlen(url));
    rv = http_connect(r);

    if (rv != LUDIS_OK)
        return rv;

    rv = http_create_request(r);

    if (rv != LUDIS_OK)
        return LUDIS_ERR;

    rv = http_fill(r);

    if (rv != LUDIS_OK)
        return LUDIS_ERR;

    return LUDIS_OK;
}

int
http_create_request(struct http_response *r)
{
    int n;
    char buf[255];

    n = snprintf(buf, 255, \
        "GET %s HTTP/1.1\r\n"
        "Host: simonklee.org\r\n"
        "Connection: Close\r\n\r\n", r->url->data);

    if (buffer_write(r->handle->wb, buf, n) != n) {
        return LUDIS_ERR;
    }

    return http_flush(r);
}

int 
http_flush(struct http_response *r)
{
    int n = buffer_len(r->handle->wb);

    if (n == 0)
        return LUDIS_OK;

    if (buffer_write_to(r->handle->wb, r->handle->fd) != n) {
        /* TODO: handle write errors */
        return LUDIS_ERR;
    }

    return LUDIS_OK;
}

int 
http_fill(struct http_response *r) 
{
    int nread;

    nread = buffer_read_from(r->handle->rb, r->handle->fd);

    if (nread <= 0) {
        /* TODO: handle response err */
        return nread;
    }

    printf("%s\n", r->handle->rb->s->data);
    http_parse(r);
    return LUDIS_OK;
}

int 
http_parse(struct http_response *r)
{
    return LUDIS_OK;
}

static int
http_connect(struct http_response *r)
{
    int rv;
    struct net_addr addr;

    rv = handle_connect_gai(r->handle, AF_UNSPEC, "simonklee.org", 80, &addr);

    if (rv != LUDIS_OK)
        return rv;

    debug_netaddr(&addr);
    return LUDIS_OK;
}

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
