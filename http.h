#ifndef LUDIS_HTTP_H
#define LUDIS_HTTP_H

#include "str.h"
#include "handle.h"
#include "addr.h"

struct http_response {
    unsigned char        flags;
    struct ludis_handle *handle;
    Str                 *url;
    Str                 *body;
};

struct http_response *http_init();
int http_free(struct http_response *r);
int http_get(struct http_response *r, const char *url);
int http_fill(struct http_response *r);
int http_create_request(struct http_response *r);
int http_flush(struct http_response *r);
int http_parse(struct http_response *r);

enum http_flags { 
    HTTP_COMPLETED  = 1 << 0,
    HTTP_WOULDBLOCK = 1 << 1
};

#endif
