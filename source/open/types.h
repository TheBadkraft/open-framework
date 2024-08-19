#ifndef _OPEN_TYPES_H
#define _OPEN_TYPES_H

#include <stdbool.h>

/*
	Pure C - no windows code
	removed any win32/cpp defines, typedefs, etc
*/

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
typedef unsigned short ushort;

typedef char sbyte;

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef B
#define B(b) b ? "true" : "false"
#endif

#endif // _OPEN_TYPES_H
