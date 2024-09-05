#ifndef _INTERNAL_STRING_H
#define _INTERNAL_STRING_H

#include <stdarg.h>

#include "../types.h"

void __str_vjoin(string, string *, va_list);
int __str_vsnjoin(string, string *, va_list);

#endif //  _INTERNAL_IO_H