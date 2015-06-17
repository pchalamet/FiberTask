#pragma once

#include "Task.h"



class CScheduler
{
private:
	typedef std::list<CTask*> STaskList;

	CTask* _activeTask;
	STaskList _tasks;
	STaskList _suspendedTasks;
	bool _stopping;
	LPVOID _fiber;
	LPUNHANDLED_EXCEPTION_ROUTINE _ueHandler;

	CTask* NextTask();
	void RunActiveTasks();
	void ReadyNonReadyTasks();

public:
	CScheduler(void);
	~CScheduler(void);

	void Run(LPUNHANDLED_EXCEPTION_ROUTINE ueHandler);
	void YieldActiveTask() const;
	void StartTask(CTask* pTask);
	void KillAllTasks();
	const CTask* CurrentTask() const;
	bool IsManagedTask(CTask* pTask) const;
};
