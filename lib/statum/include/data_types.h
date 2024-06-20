#ifndef DATA_TYPES_H
#define DATA_TYPES_H

/*
	Pure C - no windows code
	removed any win32/cpp defines, typedefs, etc
*/

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef int INT32;
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef float FLOAT;
typedef double DOUBLE;
typedef int BOOL;

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#endif	// DATA_TYPES_H
