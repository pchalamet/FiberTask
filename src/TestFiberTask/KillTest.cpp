#include "StdAfx.h"
#include "KillTest.h"


// =======================================================================================================
// =======================================================================================================

CPPUNIT_TEST_SUITE_REGISTRATION( CKillTest );




// =======================================================================================================
// =======================================================================================================


static int _killstoppedtask_ok;
static LPTASK _task_killstoppedtask_1;
static LPTASK _task_killstoppedtask_2;

static void CALLBACK Task_killstoppedtask_1(LPVOID arg)
{
	__try
	{
		while( true )
		{
			++_killstoppedtask_ok;							// reached 1 time

			FT_Pause();
		}

		++_killstoppedtask_ok;								// never reached
	}
	__finally
	{
		++_killstoppedtask_ok;								// reached
	}
}

static void CALLBACK Task_killstoppedtask_2(LPVOID arg)
{
	++_killstoppedtask_ok;								// reached

	FT_Pause();

	++_killstoppedtask_ok;								// reached

	FT_StopTask(_task_killstoppedtask_1);

	++_killstoppedtask_ok;								// reached

	FT_Pause();

	++_killstoppedtask_ok;								// reached

	FT_KillTask(_task_killstoppedtask_1);

	++_killstoppedtask_ok;								// reached
}


void CKillTest::testKillStoppedTask()
{
	_killstoppedtask_ok = 0;

	FT_Initialize();

	_task_killstoppedtask_1 = FT_NewTask("task1", Task_killstoppedtask_1, NULL);
	_task_killstoppedtask_2 = FT_NewTask("task2", Task_killstoppedtask_2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_killstoppedtask_ok == 6); // 6 when using ressource disposal
}


// =======================================================================================================
// =======================================================================================================


static int _task_self_kill_ok;
static LPTASK _task_self_kill;

static void CALLBACK Task_Self_Kill(LPVOID arg)
{
	++_task_self_kill_ok;							// reached

	while( true )
	{
		FT_KillTask(_task_self_kill);

		++_task_self_kill_ok;						// never reached
	}
}

void CKillTest::testTaskSelfKill()
{
	_task_self_kill_ok = 0;

	FT_Initialize();

	_task_self_kill = FT_NewTask("task1", Task_Self_Kill, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_self_kill_ok == 1);
}


// =======================================================================================================
// =======================================================================================================


static int _task_kill_before_ok;
static LPTASK _task_kill_before_1;
static LPTASK _task_kill_before_2;

static void CALLBACK Task_Kill_Before_1(LPVOID arg)
{
	while( true )
	{
		++_task_kill_before_ok;								// never reached
	}
}

static void CALLBACK Task_Kill_Before_2(LPVOID arg)
{
	++_task_kill_before_ok;									// reached

	FT_KillTask(_task_kill_before_1);

	++_task_kill_before_ok;									// reached

	FT_Pause();

	++_task_kill_before_ok;									// reached
}


void CKillTest::testKillTaskBeforeExecution()
{
	_task_kill_before_ok = 0;

	FT_Initialize();

	_task_kill_before_1 = FT_NewTask("task1", Task_Kill_Before_1, NULL);
	_task_kill_before_2 = FT_NewTask("task2", Task_Kill_Before_2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_kill_before_ok == 3);
}

// =======================================================================================================
// =======================================================================================================


static int _task_kill_after_ok;
static LPTASK _task_kill_after_1;
static LPTASK _task_kill_after_2;

static void CALLBACK Task_Kill_After_1(LPVOID arg)
{
	while( true )
	{
		++_task_kill_after_ok;							// reached 1 time

		FT_Pause();
	}

	++_task_kill_after_ok;								// never reached
}

static void CALLBACK Task_Kill_After_2(LPVOID arg)
{
	++_task_kill_after_ok;								// reached

	FT_Pause();

	++_task_kill_after_ok;								// reached

	FT_KillTask(_task_kill_after_1);

	++_task_kill_after_ok;								// reached

	FT_Pause();

	++_task_kill_after_ok;								// reached
}


void CKillTest::testKillTaskAfterExecution()
{
	_task_kill_after_ok = 0;

	FT_Initialize();

	_task_kill_after_1 = FT_NewTask("task1", Task_Kill_After_1, NULL);
	_task_kill_after_2 = FT_NewTask("task2", Task_Kill_After_2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_kill_after_ok == 5);
}


// =======================================================================================================
// =======================================================================================================


static int _killalltasks_ok;
static LPTASK _task_killalltasks_1;
static LPTASK _task_killalltasks_2;

static void CALLBACK Task_KillAllTasks_1(LPVOID arg)
{
	while( true )
	{
		++_killalltasks_ok;							// reached 1 time

		FT_Pause();
	}

	++_killalltasks_ok;								// never reached
}

static void CALLBACK Task_KillAllTasks_2(LPVOID arg)
{
	++_killalltasks_ok;								// reached

	FT_Pause();

	++_killalltasks_ok;								// reached

	FT_KillAllTasks();

	++_killalltasks_ok;								// never reached
}


void CKillTest::testKillAllTasks()
{
	_killalltasks_ok = 0;

	FT_Initialize();

	_task_killalltasks_1 = FT_NewTask("task1", Task_KillAllTasks_1, NULL);
	_task_killalltasks_2 = FT_NewTask("task2", Task_KillAllTasks_2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_killalltasks_ok == 3);
}


// =======================================================================================================
// =======================================================================================================



static int _killalltasksthennewtask_ok;
static LPTASK _task_killalltasksthennewtask_1;
static LPTASK _task_killalltasksthennewtask_2;
static LPTASK _task_killalltasksthennewtask_3;

static void CALLBACK Task_KillAllTasksThenNewTask_1(LPVOID arg)
{
	while( true )
	{
		++_killalltasksthennewtask_ok;							// reached 1 time

		FT_Pause();
	}

	++_killalltasksthennewtask_ok;								// never reached
}

static void CALLBACK Task_KillAllTasksThenNewTask_2(LPVOID arg)
{
	++_killalltasksthennewtask_ok;								// reached

	FT_Pause();

	++_killalltasksthennewtask_ok;								// reached

	FT_KillAllTasks();

	++_killalltasksthennewtask_ok;								// never reached
}



static void CALLBACK Task_KillAllTasksThenNewTask_3(LPVOID arg)
{
	++_killalltasksthennewtask_ok;								// reached

	FT_Pause();

	++_killalltasksthennewtask_ok;								// reached
}

void CKillTest::testKillAllTasksThenNewTask()
{
	_killalltasksthennewtask_ok = 0;

	FT_Initialize();

	_task_killalltasksthennewtask_1 = FT_NewTask("task1", Task_KillAllTasksThenNewTask_1, NULL);
	_task_killalltasksthennewtask_2 = FT_NewTask("task2", Task_KillAllTasksThenNewTask_2, NULL);

	FT_WaitNoTask();

	_task_killalltasksthennewtask_3 = FT_NewTask("task3", Task_KillAllTasksThenNewTask_3, NULL);
	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_killalltasksthennewtask_ok == 5);
}


