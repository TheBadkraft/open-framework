#ifndef _OPEN_CORE_H
#define _OPEN_CORE_H

#include <setjmp.h>
#include <signal.h>

#include "types.h"
#include "string.h"
#include "version.h"
#include "diagnostics.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef B
#define B(b) b ? "true" : "false"
#endif
#ifndef YN
#define YN(b) b ? "yes" : "no"
#endif

#endif //  _OPEN_CORE_H