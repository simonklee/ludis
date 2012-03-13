#ifndef _LUDIS_H
#define _LUDIS_H

#include <stdarg.h>

int vformat(char *str, const char *fmt, va_list ap);
int format(char *str, const char *fmt, ...);
int call(const char *fmt, ...);

#endif
