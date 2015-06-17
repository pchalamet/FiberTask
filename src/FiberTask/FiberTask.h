

#include "APISupport.h"


extern "C"
{
	typedef void CALLBACK TaskStartRoutine(LPVOID arg);
	typedef TaskStartRoutine* LPTASK_START_ROUTINE;

	typedef void CALLBACK FiberAbortRoutine();
	typedef FiberAbortRoutine* LPFIBER_ABORT_ROUTINE;

	typedef void CALLBACK UnhandledExceptionRoutine(LPEXCEPTION_RECORD pex);
	typedef UnhandledExceptionRoutine* LPUNHANDLED_EXCEPTION_ROUTINE;

	typedef void* LPTASK;

	// safe from thread *ONLY*
	void FIBERTASK_API(FT_Initialize)();
	void FIBERTASK_API(FT_Uninitialize)();
	void FIBERTASK_API(FT_WaitNoTask)();
	void FIBERTASK_API(FT_WaitNoTaskEx)(LPUNHANDLED_EXCEPTION_ROUTINE proc);
	void FIBERTASK_API(FT_KillAllTasks)();

	// safe from either fiber or thread
	LPTASK FIBERTASK_API(FT_NewTask)(LPCTSTR taskName, LPTASK_START_ROUTINE startProc, LPVOID startArg);
	LPTASK FIBERTASK_API(FT_NewTaskEx)(LPCTSTR taskName, LPTASK_START_ROUTINE startProc, LPVOID startArg, LPFIBER_ABORT_ROUTINE abortProc);
	void FIBERTASK_API(FT_Pause)();

	// safe from fiber code *ONLY*
	void FIBERTASK_API(FT_StartTask)(LPTASK pTask);
	void FIBERTASK_API(FT_StopTask)(LPTASK pTask);
	void FIBERTASK_API(FT_KillTask)(LPTASK pTask);
	LPCSTR FIBERTASK_API(FT_TaskName)();
}
