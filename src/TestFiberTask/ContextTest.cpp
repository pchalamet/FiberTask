#include "StdAfx.h"
#include "ContextTest.h"


// =======================================================================================================
// =======================================================================================================

CPPUNIT_TEST_SUITE_REGISTRATION( CContextTest );


// =======================================================================================================
// =======================================================================================================

void CContextTest::testInterface()
{
	// volontary empty
}

// =======================================================================================================
// =======================================================================================================


void CContextTest::testPauseWithoutFiber()
{
	FT_Pause();
}


// =======================================================================================================
// =======================================================================================================


void CContextTest::testWaitNoTaskWithoutTask()
{
	FT_Initialize();
	FT_WaitNoTask();
	FT_Uninitialize();
}




// =======================================================================================================
// =======================================================================================================

static bool _task_param_1;
static void CALLBACK Task_TaskParam_1(LPVOID arg)
{
	_task_param_1 = arg == (LPVOID)42;
}

void CContextTest::testTaskParam()
{
	_task_param_1 = false;

	FT_Initialize();

	FT_NewTask("task1", Task_TaskParam_1, (LPVOID)42);
	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_param_1);
}

// =======================================================================================================
// =======================================================================================================

static std::vector<LPVOID> _order_args;

static void CALLBACK Task_Order_1(LPVOID arg)
{
	_order_args.push_back(arg);

	FT_Pause();

	_order_args.push_back(arg);
}

static void CALLBACK Task_Order_2(LPVOID arg)
{
	_order_args.push_back(arg);

	FT_Pause();

	_order_args.push_back(arg);
}

void CContextTest::testNewTaskExecutionOrder()
{
	_order_args.clear();

	FT_Initialize();

	FT_NewTask("task1", Task_Order_1, (LPVOID)42);
	FT_NewTask("task2", Task_Order_2, (LPVOID)99);
	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_order_args.size() == 4);
	if( _order_args.size() == 4 )
	{
		CPPUNIT_ASSERT(_order_args[0] == (LPVOID)99);
		CPPUNIT_ASSERT(_order_args[1] == (LPVOID)42);
		CPPUNIT_ASSERT(_order_args[2] == (LPVOID)99);
		CPPUNIT_ASSERT(_order_args[3] == (LPVOID)42);
	}
}


// =======================================================================================================
// =======================================================================================================

static bool _task_name_1;
static void CALLBACK Task_name_1(LPVOID arg)
{
	LPCSTR pTaskName = FT_TaskName();

	_task_name_1 = (0 == strcmp(pTaskName, "task1"));
}

void CContextTest::testTaskName()
{
	_task_name_1 = false;

	FT_Initialize();

	FT_NewTask("task1", Task_name_1, NULL);
	FT_WaitNoTask();

	FT_Uninitialize();

    CPPUNIT_ASSERT(_task_name_1);
}
