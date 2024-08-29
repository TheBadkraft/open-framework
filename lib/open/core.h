#ifndef _OPEN_CORE_H
#define _OPEN_CORE_H

#include <setjmp.h>
#include <signal.h>

#include "types.h"
#include "string.h"
#include "version.h"

#ifndef DEBUG
#define DEBUG !NDEBUG
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef B
#define B(b) b ? "true" : "false"
#endif
#ifndef YN
#define YN(b) b ? "yes" : "no"
#endif

// extern const struct O_Core
// {
//     bool (*is_allocated)(void *);
// } Core;
#endif //  _OPEN_CORE_H