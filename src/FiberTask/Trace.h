
#pragma once

#include <stdio.h>


extern char g_TraceTxtBuf[1024];


#ifdef _DEBUG

#define TRACE_FUNC2(func, level, fmt, prm1, prm2)  \
    { sprintf_s(g_TraceTxtBuf, sizeof g_TraceTxtBuf, level ";%d;%s;" fmt "\n", ::GetCurrentThreadId(), func, prm1, prm2); \
      OutputDebugString(g_TraceTxtBuf); }

#define TRACE2(level, fmt, prm1, prm2)  TRACE_FUNC2(__FUNCTION__, level, fmt, prm1, prm2)
#define TRACE1(level, fmt, prm1)  TRACE2(level, fmt, prm1, NULL);
#define TRACE(level, txt) TRACE1(level, "%s", txt);

#define TRACE_INFO2(fmt, prm1, prm2) TRACE2("INFO", fmt, prm1, prm2);
#define TRACE_INFO1(fmt, prm1) TRACE_INFO2(fmt, prm1, NULL);
#define TRACE_INFO(txt) TRACE_INFO1("%s", txt);
#define TRACE_INFO_IF2(assert, fmt, prm1, prm2) if( (assert) ) { TRACE_INFO2(fmt, prm1, prm2); }
#define TRACE_INFO_IF1(assert, fmt, prm1) TRACE_INFO_IF2(assert, fmt, prm1, NULL);
#define TRACE_INFO_IF(assert, txt) TRACE_INFO_IF1(assert, "%s", txt);

#define TRACE_WARNING2(fmt, prm1, prm2) TRACE2("WARNING", fmt, prm1, prm2);
#define TRACE_WARNING1(fmt, prm1) TRACE_WARNING2(fmt, prm1, NULL);
#define TRACE_WARNING(txt) TRACE_WARNING1("%s", txt);
#define TRACE_WARNING_IF2(assert, fmt, prm1, prm2) if( (assert) ) { TRACE_WARNING2(fmt, prm1, prm2); }
#define TRACE_WARNING_IF1(assert, fmt, prm1) TRACE_WARNING_IF2(assert, fmt, prm1, NULL);
#define TRACE_WARNING_IF(assert, txt) TRACE_WARNING_IF1(assert, "%s", txt);

#define TRACE_ERROR2(fmt, prm1, prm2) TRACE2("ERROR", fmt, prm1, prm2);
#define TRACE_ERROR1(fmt, prm1) TRACE_ERROR2(fmt, prm1, NULL);
#define TRACE_ERROR(txt) TRACE_ERROR1("%s", txt);
#define TRACE_ERROR_IF2(assert, fmt, prm1, prm2) if( (assert) ) { TRACE_ERROR2(fmt, prm1, prm2); }
#define TRACE_ERROR_IF1(assert, fmt, prm1) TRACE_ERROR_IF2(assert, fmt, prm1, NULL);
#define TRACE_ERROR_IF(assert, txt) TRACE_ERROR_IF1(assert, "%s", txt);


#else
	#define TRACE_FUNC2(func, level, fmt, prm1, prm2)

	#define TRACE2(fmt, prm1, prm2)
	#define TRACE1(fmt, prm1)
	#define TRACE(txt)     

	#define TRACE_INFO2(fmt, prm1, prm2)
	#define TRACE_INFO1(fmt, prm1)
	#define TRACE_INFO(txt)
	#define TRACE_INFO_IF2(assert, fmt, prm1, prm2)
	#define TRACE_INFO_IF1(assert, fmt, prm1)
	#define TRACE_INFO_IF(assert, txt)

	#define TRACE_WARNING2(fmt, prm1, prm2)
	#define TRACE_WARNING1(fmt, prm1)
	#define TRACE_WARNING(txt)     
	#define TRACE_WARNING_IF2(assert, fmt, prm1, prm2)
	#define TRACE_WARNING_IF1(assert, fmt, prm1)
	#define TRACE_WARNING_IF(assert, txt)

	#define TRACE_ERROR2(fmt, prm1, prm2)
	#define TRACE_ERROR1(fmt, prm1)
	#define TRACE_ERROR(txt)     
	#define TRACE_ERROR_IF2(assert, fmt, prm1, prm2)
	#define TRACE_ERROR_IF1(assert, fmt, prm1)
	#define TRACE_ERROR_IF(assert, txt)

#endif // _DEBUG

