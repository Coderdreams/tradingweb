#include <cppunit/ui/text/TestRunner.h>
#include <Poco/Util/Application.h>
#include "HTTPServerTest.hpp"

namespace trading {

class TestApplication : public Poco::Util::Application
{
protected:
    virtual int main(const std::vector<std::string> &args)
    {
        CppUnit::TextUi::TestRunner runner;
        runner.addTest(trading::HTTPServerTest::suite());
        bool wasSucessful = runner.run( "", false );
        return wasSucessful;
    }
};

}

POCO_APP_MAIN(trading::TestApplication);
