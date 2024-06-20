#include <assert.h>

#include "fault.h"

//----------------------------------------------------------------------------
//	modified removing all Win32 references, defines, typedefs, etc
//
// FaultHandler
//----------------------------------------------------------------------------
void FaultHandler(const char* file, unsigned short line)
{
	assert(0);
}
