
#pragma once

class CResourceDisposalTest : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CResourceDisposalTest );
        CPPUNIT_TEST2( testRessourceDisposedOnKill, "FiberTask_8" );
        CPPUNIT_TEST2( testRessourceDisposedOnSuicide, "FiberTask_8" );
        CPPUNIT_TEST2( testRessourceDisposedOnReturn, "FiberTask_8" );
        CPPUNIT_TEST2( testRessourceDisposedOnStopped, "FiberTask_8" );
        CPPUNIT_TEST2( testRessourceDisposedOnKillAllTasks, "FiberTask_8" );
    CPPUNIT_TEST_SUITE_END();

	void testRessourceDisposedOnKill();
	void testRessourceDisposedOnSuicide();
	void testRessourceDisposedOnReturn();
	void testRessourceDisposedOnStopped();
	void testRessourceDisposedOnKillAllTasks();
};
