#include "StdAfx.h"
#include "Scheduler.h"
#include "Trace.h"


// ========================================================================================================

CScheduler::CScheduler(void)
{
	_stopping = false;
	_activeTask = NULL;
	_fiber = NULL;
	_ueHandler = NULL;

	TRACE_INFO("scheduler created");
}

// ========================================================================================================

CScheduler::~CScheduler(void)
{
}

// ========================================================================================================

CTask* CScheduler::NextTask()
{
	_ASSERT(0 < _tasks.size() );

	CTask* pTask = _tasks.front();
	_tasks.pop_front();
	_tasks.push_back(pTask);

	pTask->SetUnhandledExceptionHandler(_ueHandler);

	// if stopping then kill the task before running it
	if( _stopping )
	{
		pTask->Kill();
	}

	return pTask;
}

// ========================================================================================================

void CScheduler::ReadyNonReadyTasks()
{
	_tasks = _suspendedTasks;
	_suspendedTasks.clear();
}


// ========================================================================================================

void CScheduler::Run(LPUNHANDLED_EXCEPTION_ROUTINE ueHandler)
{
	// entering the dark side
	_fiber = ::ConvertThreadToFiber(NULL);
	_ueHandler = ueHandler;

	TRACE_INFO("scheduler is running");

	// run active tasks
	RunActiveTasks();

	// kill suspended tasks 
	KillAllTasks();
	ReadyNonReadyTasks();
	RunActiveTasks();

	TRACE_INFO("scheduler is stopping");

	// back to the world
	::ConvertFiberToThread();
	_fiber = NULL;
	_activeTask = NULL;
	_stopping = false;
}

// ========================================================================================================

void CScheduler::RunActiveTasks()
{
	// while there is an available task run it
	while( 0 < _tasks.size() )
	{
		// elect the next task
		_activeTask = NextTask();

		// run the task and get afterward it's new state
		CTask::ETaskState state = _activeTask->ResumeTask();

		// take a decision accordingly to the new state of the task
		switch( state )
		{
		// task is stopped ==> remove it from active task
		case CTask::Stopped:
			TRACE_INFO1("stopping task %s", _activeTask->TaskName().c_str());
			_tasks.remove(_activeTask);
			_suspendedTasks.push_back(_activeTask);
			break;

		// task s killed ==> detroy the task permanently
		case CTask::Killed:
			TRACE_INFO1("killing task %s", _activeTask->TaskName().c_str());
			delete _activeTask;
			_tasks.remove(_activeTask);
		}

		// give up the remaining CPU time to windows
		Sleep(0);
	}

	// no more task to run
	_activeTask = NULL;
}


// ========================================================================================================

void CScheduler::YieldActiveTask() const
{
	_activeTask->YieldTask();
}

// ========================================================================================================

void CScheduler::KillAllTasks()
{
	TRACE_INFO("Kill all tasks flag set");

	_stopping = true;

	if( NULL != _activeTask )
	{
		_activeTask->Kill();
	}
}

// ========================================================================================================

void CScheduler::StartTask(CTask* pTask)
{
	_ASSERT(NULL != pTask);
	TRACE_INFO1("starting task %s", pTask->TaskName().c_str());

	_suspendedTasks.remove(pTask);
	_tasks.remove(pTask);

	_tasks.push_front(pTask);		// NOTE: the task will be the next one to be scheduled

	pTask->Start();
}

// ========================================================================================================

const CTask* CScheduler::CurrentTask() const
{
	return _activeTask;
}

// ========================================================================================================

bool CScheduler::IsManagedTask(CTask* pTask) const
{
	// does the task exist in ready tasks list ?
	STaskList::const_iterator it = std::find(_tasks.begin(), _tasks.end(), pTask);
	if( _tasks.end() != it )
	{
		return true;
	}

	// does the task exist in suspended tasks list ?
	it = std::find(_suspendedTasks.begin(), _suspendedTasks.end(), pTask);
	if( _suspendedTasks.end() != it )
	{
		return true;
	}
	
	// we do not know the task
	return false;
}
