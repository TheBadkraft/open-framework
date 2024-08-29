#include <assert.h>

#include "open/types.h"
#include "../open/fault.h"

/*
	Pure C - no windows code
	removed any win32/cpp defines, typedefs, etc
*/

//----------------------------------------------------------------------------
// FaultHandler
//----------------------------------------------------------------------------
void FaultHandler(const char *file, ushort line)
{
	assert(0);
}
