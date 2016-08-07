
#include "HTTPServer.hpp"
#include "HTTPServerTest.hpp"

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace trading {

void HTTPServerTest::setUp() {}

void HTTPServerTest::tearDown() {}

void HTTPServerTest::testIdentityRequest()
{
	unsigned short port = 9191;
	trading::HTTPServer srv;
	srv.start(port);
	
	HTTPClientSession cs("localhost", port);
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/echoBody");
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	CPPUNIT_ASSERT(response.getContentLength() == body.size());
	CPPUNIT_ASSERT(response.getContentType() == "text/plain");
	CPPUNIT_ASSERT(rbody == body);
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
