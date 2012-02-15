#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void
oom(void)
{
    static const char e[] = "Out of memory\n";

    fprintf(stderr, e);
    abort();
}

void *
lmalloc(size_t size) 
{
    void *ptr;

    assert(size);
    ptr = malloc(size);

    if (ptr == NULL)
        oom();

    return ptr;
}

void *
lcalloc(size_t nmemb, size_t size)
{
    void *ptr;

    assert(size);
    ptr = calloc(nmemb, size);

    if (ptr == NULL)
        oom();

    return ptr;
}

/* a version of realloc which does not allow
 * NULL ptr */
void *
lrealloc(void *ptr, size_t size)
{
    assert(ptr);
    assert(size);
    ptr = realloc(ptr, size);

    if (ptr == NULL)
        oom();

    return ptr;

}

void
lfree(void *ptr) 
{
    assert(ptr);
    free(ptr);
}


