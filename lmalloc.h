#ifndef _LMALLOC_H
#define _LMALLOC_H

void *lmalloc(size_t size);
void *lfree(void *ptr);
void *lcalloc(size_t nmemb, size_t size);
void *lrealloc(void *ptr, size_t size);

#endif
