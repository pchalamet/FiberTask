
#pragma once

class CKillTest : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CKillTest );
		CPPUNIT_TEST( testKillStoppedTask );
		CPPUNIT_TEST2( testTaskSelfKill, "FiberTask_5" );
		CPPUNIT_TEST( testKillTaskBeforeExecution );
		CPPUNIT_TEST( testKillTaskAfterExecution );
		CPPUNIT_TEST2( testKillAllTasks, "FiberTask_7" );
		CPPUNIT_TEST2( testKillAllTasksThenNewTask, "FiberTask_7" );
    CPPUNIT_TEST_SUITE_END();

	void testKillStoppedTask();
	void testTaskSelfKill();
	void testKillTaskBeforeExecution();
	void testKillTaskAfterExecution();
	void testKillAllTasks();
	void testKillAllTasksThenNewTask();
};
