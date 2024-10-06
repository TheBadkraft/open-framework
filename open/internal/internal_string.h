#ifndef _INTERNAL_STRING_H
#define _INTERNAL_STRING_H

#include <stdarg.h>

#include "../core/types.h"

void str_vjoin(string, string *, va_list);
size_t str_vsnjoin(string, string *, va_list);

#endif //  _INTERNAL_IO_H