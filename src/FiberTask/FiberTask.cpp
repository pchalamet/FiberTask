// FiberTask.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FiberTask.h"
#include "Scheduler.h"
#include "Task.h"
#include "Trace.h"
#include "FiberEx.h"
#include "StackWalk.h"


#define FIBERTASK_GENERAL_EXCEPTION_CODE (0xE000DEAD)


static void RaiseGeneralError()
{
	::RaiseException(FIBERTASK_GENERAL_EXCEPTION_CODE, 0, 0, NULL);
}


#ifdef _DEBUG
#define FATAL_TRACE_ERROR_IF(cond, msg) { \
											TRACE_ERROR_IF(cond, msg); \
											if(cond) \
												RaiseGeneralError(); \
										}

#define FATAL_TRACE_WARNING_IF(cond, msg) { \
											TRACE_WARNING_IF(cond, msg); \
											if(cond) \
												RaiseGeneralError(); \
										}
#else
#define FATAL_TRACE_ERROR_IF(cond, msg)
#endif



// ========================================================================================================
// ========================================================================================================

static CScheduler* _pScheduler = NULL;

// ========================================================================================================
// ========================================================================================================

#ifdef _DEBUG

static bool _outputCallStackInitialized = false;

void OutputCallstack(LPCTSTR funcName)
{
	if( ! _outputCallStackInitialized )
	{
		_outputCallStackInitialized = true;

		// initialize stackwalk
		std::vector<SModuleInfo> moduleInfos;
		LoadSymbols(moduleInfos);
	}

	TCallStack callstack;
	DWORD cookie = TraceCallStack(callstack);

	std::string sep = "";
	std::ostringstream outstr;
	outstr << std::hex;
	for(TCallStack::iterator it = callstack.begin();
		it != callstack.end();
		++it)
	{
		outstr << sep;
		outstr << *it;

		sep = ":";
	}

	TRACE_FUNC2(funcName, "INFO", "%s", outstr.str().c_str(), NULL);
}
#endif

#ifdef _DEBUG
	#define DEBUG_OUTPUT_CALLSTACK() OutputCallstack(__FUNCTION__)
#else
	#define DEBUG_OUTPUT_CALLSTACK()
#endif



void FIBERTASK_API(FT_Initialize)()
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(IsThreadAFiber(), "Function called inside of a fiber");
	TRACE_INFO_IF(NULL != _pScheduler, "FT_Initialize already called");

	if( NULL == _pScheduler )
	{
		_pScheduler = new CScheduler();
	}	
}

// ========================================================================================================

void FIBERTASK_API(FT_Uninitialize)()
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(IsThreadAFiber(), "Function called inside of a fiber");
	TRACE_INFO_IF(NULL == _pScheduler, "FT_Uninitialize already called");

	if( NULL != _pScheduler )
	{
		delete _pScheduler;
		_pScheduler = NULL;
	}
}

// ========================================================================================================

LPTASK FIBERTASK_API(FT_NewTask)(LPCTSTR taskName, LPTASK_START_ROUTINE proc, LPVOID startArg)
{
	return FT_NewTaskEx(taskName, proc, startArg, NULL);
}

// ========================================================================================================

LPTASK FIBERTASK_API(FT_NewTaskEx)(LPCTSTR taskName, LPTASK_START_ROUTINE proc, LPVOID startArg, LPFIBER_ABORT_ROUTINE abortProc)
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");
	FATAL_TRACE_ERROR_IF(NULL == proc, "invalid task routine");

	CTask* pTask = new CTask(taskName, proc, startArg, abortProc);
	_pScheduler->StartTask(pTask);
	return pTask;
}
// ========================================================================================================

void FIBERTASK_API(FT_Pause)()
{
	DEBUG_OUTPUT_CALLSTACK();

	if( ! IsThreadAFiber() )
	{
		TRACE_WARNING("Function called outside of a fiber");
		return;
	}

	// nop if invalid usage
	if( NULL == _pScheduler )
	{
		TRACE_WARNING("Call FT_Initialize prior calling this function");
		return;
	}

	_pScheduler->YieldActiveTask();
}

// ========================================================================================================

void FIBERTASK_API(FT_KillTask)(LPTASK task)
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(! IsThreadAFiber(), "Function called outside of fiber");
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");

	CTask* pTask = reinterpret_cast<CTask *>(task);

	if( ! _pScheduler->IsManagedTask(pTask) )
	{
		TRACE_WARNING("Invalid task for FT_KillTask");
		return;
	}

	pTask->Kill();
	_pScheduler->StartTask(pTask);
}

// ========================================================================================================

void FIBERTASK_API(FT_StopTask)(LPTASK task)
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(! IsThreadAFiber(), "Function called outside of fiber");
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");

	CTask* pTask = reinterpret_cast<CTask *>(task);

	if( ! _pScheduler->IsManagedTask(pTask) )
	{
		TRACE_WARNING("Invalid task for FT_StopTask");
		return;
	}

	pTask->Stop();
}

// ========================================================================================================

void FIBERTASK_API(FT_StartTask)(LPTASK task)	
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(! IsThreadAFiber(), "Function called outside of fiber");
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");

	_ASSERT(NULL != _pScheduler);
	_ASSERT(NULL != task);

	CTask* pTask = reinterpret_cast<CTask *>(task);

	if( ! _pScheduler->IsManagedTask(pTask) )
	{
		TRACE_ERROR("Invalid task for FT_StartTask");
		return;
	}

	_pScheduler->StartTask(pTask);

}

// ========================================================================================================

void FIBERTASK_API(FT_WaitNoTask)()
{
	FT_WaitNoTaskEx(NULL);
}

void FIBERTASK_API(FT_WaitNoTaskEx)(LPUNHANDLED_EXCEPTION_ROUTINE proc)
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");
	FATAL_TRACE_ERROR_IF(IsThreadAFiber(), "Function called inside of fiber");

	if( NULL != _pScheduler )
	{
		_pScheduler->Run(proc);
	}
}



// ========================================================================================================

void FIBERTASK_API(FT_KillAllTasks)()
{
	DEBUG_OUTPUT_CALLSTACK();

	FATAL_TRACE_ERROR_IF(! IsThreadAFiber(), "Function called outside of fiber");
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");

	if( NULL != _pScheduler )
	{
		_pScheduler->KillAllTasks();
	}
}

// ========================================================================================================

LPCSTR FIBERTASK_API(FT_TaskName)()
{
	DEBUG_OUTPUT_CALLSTACK();
	FATAL_TRACE_ERROR_IF(NULL == _pScheduler, "Call FT_Initialize prior calling this function");
	FATAL_TRACE_ERROR_IF(! IsThreadAFiber(), "Function called outside of fiber");

	const CTask* activeTask = _pScheduler->CurrentTask();
	return activeTask->TaskName().c_str();
}
