#include <assert.h>

#include "op/fault.h"

/*
	Pure C - no windows code
	removed any win32/cpp defines, typedefs, etc
*/

//----------------------------------------------------------------------------
// FaultHandler
//----------------------------------------------------------------------------
void FaultHandler(const char* file, unsigned short line)
{
	assert(0);
}
