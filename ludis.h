#ifndef LUDIS_H
#define LUDIS_H

#include <stdarg.h>

#include "str.h"

int vformat(Str **str, const char *fmt, va_list ap);
int format(Str **str, const char *fmt, ...);
int call(const char *fmt, ...);

int int_len(int n);
#endif
