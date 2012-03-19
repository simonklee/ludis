#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "deps/http_parser/http_parser.h"
#include "addr.h"
#include "common.h"
#include "lmalloc.h"
#include "http.h"
#include "str.h"

static int http_connect(struct http_response *r);
/*Str *url_domainname(Str *url);*/
/*
static void
debug_cb(const char *name, const char *buf, size_t len)
{
    Str *s = str_new_str(buf, len);
    printf("%s: %s\n", name, s->data);
    str_free(s);

}

int
request_url_cb(http_parser *p, const char *buf, size_t len)
{
    debug_cb("request_url_cb", buf, len);
    assert(p && buf && len);
    return 0;
}

int
header_field_cb(http_parser *p, const char *buf, size_t len)
{
    debug_cb("header_field_cb", buf, len);
    assert(p && buf && len);
    return 0;
}

int
header_value_cb(http_parser *p, const char *buf, size_t len)
{
    debug_cb("header_value_cb", buf, len);
    assert(p && buf && len);
    return 0;
}

int
count_body_cb(http_parser *p, const char *buf, size_t len)
{
    debug_cb("header_value_cb", buf, len);
    return 0;
}

int
message_begin_cb(http_parser *p)
{
    printf("message_begin\n");
    return 0;
}*/

int
message_complete_cb(http_parser *p)
{
    struct http_response *r = (struct http_response *) p->data;
    r->flags |= HTTP_COMPLETED;
    return 0;
}

int
body_cb(http_parser *p, const char *buf, size_t len)
{
    struct http_response *r = (struct http_response *) p->data;

    if (buffer_write(r->body, buf, len) != (int)len)
        return LUDIS_ERR;

    return LUDIS_OK;
}

int
headers_complete_cb(http_parser *p)
{
    struct http_response *r = (struct http_response *) p->data;

    r->body = buffer_new((int)p->content_length);
    return LUDIS_OK;
}

http_parser_settings parser_settings = {
    0,
    0,
    0,
    0,
    headers_complete_cb,
    body_cb,
    message_complete_cb
};

struct http_response *
http_init() 
{
    struct http_response *r;

    r = lmalloc(sizeof(struct http_response));
    r->handle = handle_new();
    r->url = NULL;
    r->body = NULL;
    r->flags = 0;
    r->parser = malloc(sizeof(struct http_parser));
    http_parser_init(r->parser, HTTP_RESPONSE);
    r->parser->data = r;

    return r;
}

int 
http_free(struct http_response *r)
{
    handle_free(r->handle);

    if (r->url) str_free(r->url);
    if (r->body) buffer_free(r->body);
    if (r->parser) free(r->parser);

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

    return http_parse(r);
}

int 
http_parse(struct http_response *r)
{
    Buffer *b = r->handle->rb;
    int n, nparse;

    if (r->flags & HTTP_READING) {
        printf("isreading\n");
    }

    n = buffer_len(b);
    nparse = http_parser_execute(r->parser, &parser_settings, b->s->data, n);

    if (r->flags & HTTP_COMPLETED) {
        printf("iscomplete\n");
    }

    if (n != nparse) {
        fprintf(stderr, "expected %d got %d\n", n, nparse);
        return LUDIS_ERR;
    }

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

/*static Str *
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
}*/
