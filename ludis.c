#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "ludis.h"

int 
call(char *fmt, ...)
{
    assert(fmt != NULL);
    return LUDIS_OK;
}
