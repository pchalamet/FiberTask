
#pragma once

class CStopTest : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CStopTest );
        CPPUNIT_TEST2( testTaskSelfStop, "FiberTask_4" );
        CPPUNIT_TEST( testStopTaskBeforeExecution );
        CPPUNIT_TEST( testStopTaskAfterExecution );
    CPPUNIT_TEST_SUITE_END();

	void testTaskSelfStop();
	void testStopTaskBeforeExecution();
	void testStopTaskAfterExecution();
};
