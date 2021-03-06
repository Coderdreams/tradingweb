#include <cppunit/ui/text/TestRunner.h>
#include <Poco/Util/Application.h>
#include "Poco/Process.h"
#include "HTTPServerTest.hpp"
#include <iostream>

namespace trading {

class TestApplication : public Poco::Util::Application
{
private:
	std::unique_ptr<Poco::ProcessHandle> _ph;

public:

	virtual ~TestApplication() {
		Poco::Process::requestTermination(_ph->id()); // Poco should terminate the process alone
		std::cout << "Cleaning up and finishing" << std::endl;
	}

	TestApplication() {
		std::string name("./tradingApp");
		Poco::Process::Args args;
		_ph = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(name, args));
		usleep(15000); // FIXME: I have no way of determining that the process is listening on the port yet
	}

protected:
    virtual int main(const std::vector<std::string> &args)
    {
        CppUnit::TextUi::TestRunner runner;
        runner.addTest(trading::HTTPServerTest::suite());
        bool wasSucessful = runner.run( "", false );
        return wasSucessful;
    }
};

} // namespace: trading

POCO_APP_MAIN(trading::TestApplication)
