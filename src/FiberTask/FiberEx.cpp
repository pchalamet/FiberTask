
#include "stdafx.h"
#include "FiberEx.h"



#if _WIN32_WINNT < 0x600

BOOL IsThreadAFiber()
{
	LPVOID fiber = GetCurrentFiber();
	if( fiber == (LPVOID)0x1e00
		|| fiber == (LPVOID)0 )
	{
		return FALSE;
	}

	return TRUE;
}

#endif


