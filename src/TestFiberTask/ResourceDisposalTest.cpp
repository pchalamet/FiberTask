#include "StdAfx.h"
#include "ResourceDisposalTest.h"


// =======================================================================================================
// =======================================================================================================

//CPPUNIT_TEST_SUITE_REGISTRATION( CResourceDisposalTest );


// =======================================================================================================
// =======================================================================================================

static LPTASK _task_resource_disposed_kill1;
static LPTASK _task_resource_disposed_kill2;
static int _resource_disposed_kill_ok;

static void CALLBACK Task_Resource_Disposed_kill1(LPVOID arg)
{
	++_resource_disposed_kill_ok;					// reached

	__try
	{
		while( true )
		{
			++_resource_disposed_kill_ok;			// reached 1 time
			FT_Pause();
		}

		++_resource_disposed_kill_ok;				// never reached
	}
	__finally
	{
		++_resource_disposed_kill_ok;				// reached
	}

	++_resource_disposed_kill_ok;					// never reached
}

static void CALLBACK Task_Resource_Disposed_kill2(LPVOID arg)
{
	++_resource_disposed_kill_ok;					// reached

	__try
	{
		++_resource_disposed_kill_ok;				// reached

		FT_Pause();

		++_resource_disposed_kill_ok;				// reached

		FT_KillTask(_task_resource_disposed_kill1);

		++_resource_disposed_kill_ok;				// reached
	}
	__finally
	{
		++_resource_disposed_kill_ok;				// reached
	}

	++_resource_disposed_kill_ok;					// reached
}

void CResourceDisposalTest::testRessourceDisposedOnKill()
{
	_resource_disposed_kill_ok = 0;
	
	FT_Initialize();

	_task_resource_disposed_kill1 = FT_NewTask("task1", Task_Resource_Disposed_kill1, NULL);
	_task_resource_disposed_kill2 = FT_NewTask("task2", Task_Resource_Disposed_kill2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_resource_disposed_kill_ok == 9);
}


// =======================================================================================================
// =======================================================================================================

static LPTASK _task_resource_disposed_suicide;
static int _resource_disposed_suicide_ok;

static void CALLBACK Task_Resource_Disposed_suicide(LPVOID arg)
{
	++_resource_disposed_suicide_ok;					// reached

	__try
	{
		++_resource_disposed_suicide_ok;				// reached

		FT_KillTask(_task_resource_disposed_suicide);

		++_resource_disposed_suicide_ok;				// never reached
	}
	__finally
	{
		++_resource_disposed_suicide_ok;				// reached
	}

	++_resource_disposed_suicide_ok;					// never reached
}

void CResourceDisposalTest::testRessourceDisposedOnSuicide()
{
	_resource_disposed_suicide_ok = 0;
	
	FT_Initialize();

	_task_resource_disposed_suicide = FT_NewTask("task1", Task_Resource_Disposed_suicide, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_resource_disposed_suicide_ok == 3);
}

// =======================================================================================================
// =======================================================================================================


static LPTASK _task_resource_disposed_return;
static int _resource_disposed_return_ok;

static void CALLBACK Task_Resource_Disposed_return(LPVOID arg)
{
	++_resource_disposed_return_ok;					// reached

	__try
	{
		++_resource_disposed_return_ok;				// reached
	}
	__finally
	{
		++_resource_disposed_return_ok;				// reached
	}

	++_resource_disposed_return_ok;					// reached
}

void CResourceDisposalTest::testRessourceDisposedOnReturn()
{
	_resource_disposed_return_ok = 0;
	
	FT_Initialize();

	_task_resource_disposed_return = FT_NewTask("task1", Task_Resource_Disposed_return, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_resource_disposed_return_ok == 4);
}


// =======================================================================================================
// =======================================================================================================

static LPTASK _task_resource_disposed_stopped1;
static LPTASK _task_resource_disposed_stopped2;
static int _resource_disposed_stopped_ok;

static void CALLBACK Task_Resource_Disposed_stopped1(LPVOID arg)
{
	++_resource_disposed_stopped_ok;					// reached

	__try
	{
		while( true )
		{
			++_resource_disposed_stopped_ok;			// reached 1 time
			FT_Pause();
		}

		++_resource_disposed_stopped_ok;				// never reached
	}
	__finally
	{
		++_resource_disposed_stopped_ok;				// reached
	}

	++_resource_disposed_stopped_ok;					// never reached
}

static void CALLBACK Task_Resource_Disposed_stopped2(LPVOID arg)
{
	++_resource_disposed_stopped_ok;					// reached

	__try
	{
		++_resource_disposed_stopped_ok;				// reached

		FT_Pause();

		++_resource_disposed_stopped_ok;				// reached

		FT_StopTask(_task_resource_disposed_stopped1);

		++_resource_disposed_stopped_ok;				// reached
	}
	__finally
	{
		++_resource_disposed_stopped_ok;				// reached
	}

	++_resource_disposed_stopped_ok;					// reached
}

void CResourceDisposalTest::testRessourceDisposedOnStopped()
{
	_resource_disposed_stopped_ok = 0;
	
	FT_Initialize();

	_task_resource_disposed_stopped1 = FT_NewTask("task1", Task_Resource_Disposed_stopped1, NULL);
	_task_resource_disposed_stopped2 = FT_NewTask("task2", Task_Resource_Disposed_stopped2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_resource_disposed_stopped_ok == 9);
}


// =======================================================================================================
// =======================================================================================================



static LPTASK _task_resource_disposed_killalltasks1;
static LPTASK _task_resource_disposed_killalltasks2;
static int _resource_disposed_killalltasks_ok;

static void CALLBACK Task_Resource_Disposed_killalltasks1(LPVOID arg)
{
	++_resource_disposed_killalltasks_ok;					// reached

	__try
	{
		while( true )
		{
			++_resource_disposed_killalltasks_ok;			// reached 1 time
			FT_Pause();
		}

		++_resource_disposed_killalltasks_ok;				// never reached
	}
	__finally
	{
		++_resource_disposed_killalltasks_ok;				// reached
	}

	++_resource_disposed_killalltasks_ok;					// never reached
}

static void CALLBACK Task_Resource_Disposed_killalltasks2(LPVOID arg)
{
	++_resource_disposed_killalltasks_ok;					// reached

	__try
	{
		++_resource_disposed_killalltasks_ok;				// reached

		FT_Pause();

		++_resource_disposed_killalltasks_ok;				// reached

		FT_KillAllTasks();

		++_resource_disposed_killalltasks_ok;				// never reached
	}
	__finally
	{
		++_resource_disposed_killalltasks_ok;				// reached
	}

	++_resource_disposed_killalltasks_ok;					// never reached
}

void CResourceDisposalTest::testRessourceDisposedOnKillAllTasks()
{
	_resource_disposed_killalltasks_ok = 0;
	
	FT_Initialize();

	_task_resource_disposed_killalltasks1 = FT_NewTask("task1", Task_Resource_Disposed_killalltasks1, NULL);
	_task_resource_disposed_killalltasks2 = FT_NewTask("task2", Task_Resource_Disposed_killalltasks2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_resource_disposed_killalltasks_ok == 7);
}

