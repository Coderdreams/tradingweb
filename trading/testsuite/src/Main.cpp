#include <cppunit/ui/text/TestRunner.h>
#include "HTTPServerTest.hpp"

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(trading::HTTPServerTest::suite());
    bool wasSucessful = runner.run( "", false );
    return wasSucessful;
}

