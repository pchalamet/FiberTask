// TestFiberTask.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

using namespace std;

class MyXmlOutputterHook : public CPPUNIT_NS::XmlOutputterHook
{
    public:
        MyXmlOutputterHook(const std::string projectName, const std::string author)
        {
            m_projectName = projectName;
            m_author      = author;
        };
 
        virtual ~MyXmlOutputterHook()
        {
        };
 
        void beginDocument(CPPUNIT_NS::XmlDocument* document)
        {
            if (!document)
            return;

            char tmpbuf[128];
            SYSTEMTIME st;
            GetSystemTime(&st);
            wsprintf(tmpbuf, "%u/%u/%u",st.wDay, st.wMonth, st.wYear);

            CPPUNIT_NS::XmlElement* metaEl = new CPPUNIT_NS::XmlElement("SuiteInfo", "");
            metaEl->addElement( new CPPUNIT_NS::XmlElement("Author", m_author) );
            metaEl->addElement( new CPPUNIT_NS::XmlElement("Project", m_projectName) );
            metaEl->addElement( new CPPUNIT_NS::XmlElement("Date", tmpbuf ) );
            document->rootElement().addElement(metaEl);
        };
    private:
        string m_projectName;
        string m_author;
}; 

int _tmain(int argc, _TCHAR* argv[])
{
    // Run the test.
    bool wasSucessful = false;

    // Get the top level suite from the registry
    CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

    // Adds the test to the list of test to run
    CPPUNIT_NS::TextUi::TestRunner runner;
    runner.addTest( suite );

    // Change the default outputter to a compiler error format outputter
    if ( 2 == argc )
    {
        std::ofstream file( "TestFiberTask.xml" );

        CPPUNIT_NS::XmlOutputter* outputter = new CPPUNIT_NS::XmlOutputter( &runner.result(), file );    
        MyXmlOutputterHook hook("FiberTask", argv[1]);
        outputter->addHook(&hook);
        runner.setOutputter(outputter);    
        wasSucessful = runner.run();
        file.close();
    }
    else
    {
        runner.setOutputter( new CPPUNIT_NS::XmlOutputter( &runner.result(), CPPUNIT_NS::stdCOut() ) );
        wasSucessful = runner.run();
    }

    // Return error code 1 if the one of test failed.
    return wasSucessful ? 0 : 1;
}

