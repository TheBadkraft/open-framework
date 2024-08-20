#ifndef _OPEN_CORE_H
#define _OPEN_CORE_H

#include "../open.h"

#include "types.h"
#include "string.h"

#ifndef DEBUG
#define DEBUG !NDEBUG
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef B
#define B(b) b ? "true" : "false"
#endif

#endif //  _OPEN_CORE_H