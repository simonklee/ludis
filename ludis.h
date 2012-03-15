#ifndef LUDIS_H
#define LUDIS_H

#include <stdarg.h>

int vformat(char **str, const char *fmt, va_list ap);
int format(char **str, const char *fmt, ...);
int call(const char *fmt, ...);

int int_len(int n);
#endif
