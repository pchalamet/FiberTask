#include "StdAfx.h"
#include "Task.h"
#include "FiberTask.h"
#include "Scheduler.h"
#include "Trace.h"


// ========================================================================================================
// ========================================================================================================

#define TASK_ABORT_EXCEPTION_CODE (0xE0000002)


// ========================================================================================================
// ========================================================================================================


CTask::CTask(LPCTSTR taskName, LPTASK_START_ROUTINE proc, LPVOID arg, LPFIBER_ABORT_ROUTINE abortProc)
{
	_ASSERT(NULL != proc);

	_taskName = taskName;
	_proc = proc;
	_arg = arg;
	_state = Stopped;
	_ueHandler = NULL;
	_abortProc = abortProc;

	_fiber = ::CreateFiber(0, FiberProc, this);
	_ASSERT( NULL != _fiber );

	TRACE_INFO1("fiber %s created", _taskName.c_str());
}

// ========================================================================================================

CTask::~CTask(void)
{
	_ASSERT(_fiber != ::GetCurrentFiber() );
	::DeleteFiber(_fiber);

	_fiber = NULL;

	TRACE_INFO1("fiber %s destroyed", _taskName.c_str());
}

// ========================================================================================================

void CALLBACK CTask::FiberProc(LPVOID lpFiberParameter)
{
	_ASSERT(NULL != lpFiberParameter);
	CTask* pTask = reinterpret_cast<CTask *>(lpFiberParameter);

	TRACE_INFO1("fiber %s is starting", pTask->_taskName.c_str());

	pTask->Execute();
}

// ========================================================================================================

void CTask::Execute()
{
	__try
	{
		ExecuteAbortable();
	}
	__finally
	{
		TRACE_INFO1("fiber %s is finalizing", _taskName.c_str());

		// this is a no return call
		_state = Killed;
		YieldTask();

		// unless there is a bug !
		_ASSERT(FALSE);
	}
}

// ========================================================================================================

void CTask::ExecuteAbortable() const
{
	__try
	{
		YieldTask();

		_ASSERT(NULL != _proc);
		_proc(_arg);
	}
	__except(ManageUnhandledException(GetExceptionInformation()))
	{
		// swallow cancellation exception
		TRACE_INFO1("abort exception for fiber %s trapped", _taskName.c_str());
	}
}

// ========================================================================================================

DWORD CTask::ManageUnhandledException(LPEXCEPTION_POINTERS pex) const
{
	//return FilterTaskAbortException(pex);
	TRACE_INFO1("unhandled exception detected on task %s", _taskName.c_str());

	if( NULL != _ueHandler )
	{
		TRACE_INFO1("calling unhandled exception handler for task %s", _taskName.c_str());
		_ueHandler(pex->ExceptionRecord);
		TRACE_INFO1("returning from unhandled exception handler for task %s", _taskName.c_str());
	}

	// swallow exception
	return EXCEPTION_EXECUTE_HANDLER;
}

// ========================================================================================================

void CTask::Start()
{
	TRACE_INFO1("starting fiber %s", _taskName.c_str());

	if( IsAlive() )
	{
		_state = Ready;
	}
}

// ========================================================================================================

void CTask::Stop()
{
	TRACE_INFO1("stopping fiber %s", _taskName.c_str());

	if( IsAlive() )
	{
		_state = Stopped;

		// if the task is the current active task then yield it
		if( ::GetCurrentFiber() == _fiber )
		{
			YieldTask();
		}
	}
}

// ========================================================================================================

void CTask::Kill()
{
	TRACE_INFO1("killing fiber %s", _taskName.c_str());

	if( IsAlive() )
	{
		_state = Killed;

		// suicide case - just go away immediately
		if( ::GetCurrentFiber() == _fiber )
		{
			AbortTask();
		}
	}
}

// ========================================================================================================

void CTask::AbortTask() const
{
	TRACE_INFO1("aborting fiber %s", _taskName.c_str());

	//::RaiseException(TASK_ABORT_EXCEPTION_CODE, 0, 0, NULL);
	if( NULL != _abortProc )
	{
		TRACE_INFO1("calling abort callback of task %s", _taskName.c_str());
		_abortProc();
		TRACE_INFO1("returning from abort callback of task %s", _taskName.c_str());
	}

	// we have to yield there because if the _abortProc did not abort the task using an exception
	// then we have to give up the execution flow to the scheduler in order to be destroyed
	// since AbortTask() is called by Kill() which set the Kill flag
	YieldTask();
}


// ========================================================================================================

// NOTE: this function always run in the context of the scheduler
CTask::ETaskState CTask::ResumeTask()
{
	_callingFiber = ::GetCurrentFiber();

	// run only the fiber if the task is ready
	if( Stopped != _state )
	{
		::SwitchToFiber(_fiber);
	}

	// we are back in the fiber of the scheduler
	// return the state of the task
	TRACE_INFO2("fiber %s is %d", _taskName.c_str(), (DWORD)_state);
	return _state;
}


// ========================================================================================================

// NOTE: this function always run in the context of the task
void CTask::YieldTask() const
{
	TRACE_INFO1("yielding fiber %s", _taskName.c_str());

	::SwitchToFiber(_callingFiber);

	// we are back in the fiber of the task
	// if we were killed meanwhile just unwind the stack and go away
	if( ! IsAlive() )
	{
		// this is a no return call
		AbortTask();

		// unless there is a bug !
		_ASSERT(FALSE);
	}
}

// ========================================================================================================

const std::string& CTask::TaskName() const
{
	return _taskName;
}

// ========================================================================================================

bool CTask::IsAlive() const
{
	return Killed != _state;
}

// ========================================================================================================

void CTask::SetUnhandledExceptionHandler(LPUNHANDLED_EXCEPTION_ROUTINE ueHandler)
{
	_ueHandler = ueHandler;
}
