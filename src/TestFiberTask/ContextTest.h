
#pragma once

class CContextTest : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CContextTest );
        CPPUNIT_TEST2( testInterface, "FiberTask_1" );
        CPPUNIT_TEST2( testPauseWithoutFiber, "FiberTask_3" );
        CPPUNIT_TEST2( testWaitNoTaskWithoutTask, "FiberTask_6" );
        CPPUNIT_TEST( testTaskParam );
        CPPUNIT_TEST2( testNewTaskExecutionOrder, "FiberTask_2" );
        CPPUNIT_TEST( testTaskName );
    CPPUNIT_TEST_SUITE_END();

	void testInterface();
	void testPauseWithoutFiber();
	void testWaitNoTaskWithoutTask();
	void testTaskParam();
	void testNewTaskExecutionOrder();
	void testTaskName();
};
