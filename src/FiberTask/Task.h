#pragma once

#include "FiberTask.h"


class CScheduler;






class CTask
{
public:
	CTask(LPCTSTR taskName, LPTASK_START_ROUTINE proc, LPVOID arg, LPFIBER_ABORT_ROUTINE abortProc);
	~CTask();

	typedef enum ETaskState
	{
		Ready = 1 << 0,
		Stopped = 1 << 1,
		Killed = 1 << 2
	};

	ETaskState ResumeTask();
	void YieldTask() const;
	const std::string& TaskName() const;

	void Start();
	void Stop();
	void Kill();

	void SetUnhandledExceptionHandler(LPUNHANDLED_EXCEPTION_ROUTINE ueHandler);

private:
	LPVOID _fiber;
	LPVOID _callingFiber;
	LPTASK_START_ROUTINE _proc;
	std::string _taskName;
	ETaskState _state;
	LPVOID _arg;
	LPFIBER_ABORT_ROUTINE _abortProc;
	LPUNHANDLED_EXCEPTION_ROUTINE _ueHandler;

	static void CALLBACK FiberProc(LPVOID lpFiberParameter);
	void Execute();
	void ExecuteAbortable() const;
	void AbortTask() const;
	bool IsAlive() const;
	DWORD ManageUnhandledException(LPEXCEPTION_POINTERS pex) const;
};

