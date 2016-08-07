
#include "HTTPServerTest.hpp"

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/LoggingSubsystem.h"
#include <string>
#include <iostream>

namespace trading {

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Util::Application;
using Poco::Logger;
using namespace std::string_literals;

void HTTPServerTest::setUp() {
	std::string name("./tradingApp");
	Poco::Process::Args args;
	_ph = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(name, args));
}

void HTTPServerTest::tearDown() {}

void HTTPServerTest::testIdentityRequest()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/registerTrader");
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;

	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;

	std::cout << "hi";
	std::cout << rbody << "\n done \n";
			std::cout << response.getStatus() << " \n"; 
	CPPUNIT_ASSERT_EQUAL(response.getContentLength(), (int) body.size());
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "text/plain"s);
	CPPUNIT_ASSERT_EQUAL(rbody, body);
}

CppUnit::Test* HTTPServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPServerTest");

	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testIdentityRequest",
		&HTTPServerTest::testIdentityRequest
	));

	return pSuite;
}

} // namespace: trading
