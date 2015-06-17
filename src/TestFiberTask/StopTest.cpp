#include "StdAfx.h"
#include "StopTest.h"


// =======================================================================================================
// =======================================================================================================

CPPUNIT_TEST_SUITE_REGISTRATION( CStopTest );



// =======================================================================================================
// =======================================================================================================


static int _task_self_stop_ok;
static LPTASK _task_self_stop;

static void CALLBACK Task_Self_Stop(LPVOID arg)
{
	++_task_self_stop_ok;							// reached
	while( true )
	{
		FT_StopTask(_task_self_stop);

		++_task_self_stop_ok;						// never reached
	}
}

void CStopTest::testTaskSelfStop()
{
	_task_self_stop_ok = 0;

	FT_Initialize();

	_task_self_stop = FT_NewTask("task1", Task_Self_Stop, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_self_stop_ok == 1);
}


// =======================================================================================================
// =======================================================================================================



static int _task_stop_before_ok;
static LPTASK _task_stop_before_1;
static LPTASK _task_stop_before_2;

static void CALLBACK Task_Stop_Before_1(LPVOID arg)
{
	while( true )
	{
		++_task_stop_before_ok;							// never reached
	}

	++_task_stop_before_ok;								// never reached
}

static void CALLBACK Task_Stop_Before_2(LPVOID arg)
{
	++_task_stop_before_ok;								// reached

	FT_StopTask(_task_stop_before_1);

	++_task_stop_before_ok;								// reached

	FT_Pause();

	++_task_stop_before_ok;								// reached
}


void CStopTest::testStopTaskBeforeExecution()
{
	_task_stop_before_ok = 0;

	FT_Initialize();

	_task_stop_before_1 = FT_NewTask("task1", Task_Stop_Before_1, NULL);
	_task_stop_before_2 = FT_NewTask("task2", Task_Stop_Before_2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_stop_before_ok == 3);
}

// =======================================================================================================
// =======================================================================================================


static int _task_stop_after_ok;
static LPTASK _task_stop_after_1;
static LPTASK _task_stop_after_2;

static void CALLBACK Task_Stop_After_1(LPVOID arg)
{
	while( true )
	{
		++_task_stop_after_ok;							// reached 1 time

		FT_Pause();
	}

	++_task_stop_after_ok;								// never reached
}

static void CALLBACK Task_Stop_After_2(LPVOID arg)
{
	++_task_stop_after_ok;								// reached

	FT_Pause();

	++_task_stop_after_ok;								// reached

	FT_StopTask(_task_stop_after_1);

	++_task_stop_after_ok;								// reached

	FT_Pause();

	++_task_stop_after_ok;								// reached
}


void CStopTest::testStopTaskAfterExecution()
{
	_task_stop_after_ok = 0;

	FT_Initialize();

	_task_stop_after_1 = FT_NewTask("task1", Task_Stop_After_1, NULL);
	_task_stop_after_2 = FT_NewTask("task2", Task_Stop_After_2, NULL);

	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_stop_after_ok == 5);
}

