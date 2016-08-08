
#include "HTTPServerTest.hpp"

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/LoggingSubsystem.h"
#include "Poco/StreamCopier.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace trading {

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Util::Application;
using Poco::Logger;
using namespace std::string_literals;

HTTPServerTest::HTTPServerTest() {
	std::string name("./tradingApp");
	Poco::Process::Args args;
	_ph = std::make_unique<Poco::ProcessHandle>(Poco::Process::launch(name, args));
	usleep(15000); // FIXME: I have no way of determining that the process is listening on the port yet
}

HTTPServerTest::~HTTPServerTest() {
	Poco::Process::requestTermination(_ph->id()); // Poco should terminate the process alone
}

void HTTPServerTest::setUp() {}

void HTTPServerTest::tearDown() {}

void HTTPServerTest::testThrowsErrorOnInvalidRequest()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/registerTrader");
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;

	char expectedBody[] = "{\"success\": false}";
	HTTPResponse response;
	
	std::istream& is = cs.receiveResponse(response);
	//cs.receiveResponse(response) >> rbody; // FIXME: this doesn't work and I had to resort to c chars, maybe a bug in POCO

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';

	CPPUNIT_ASSERT_EQUAL(response.getContentLength(), (int) strlen(expectedBody));
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

void HTTPServerTest::testSavesTrader()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	std::string testtrader("testtrader");
	std::string body("username=" + testtrader + "&pass=asdasd");

	HTTPRequest request(HTTPRequest::HTTP_POST, "/registerTrader");

	// FIXME: this should be helpful but it isn't working
	//Poco::Net::HTMLForm form(Poco::Net::HTMLForm::ENCODING_MULTIPART); 
	//form.add("username", "user1"); 
	//form.add("pass", "test123");
	//form.prepareSubmit(request); 

	request.setContentLength((int) body.length());
	request.setContentType("application/json");
	cs.sendRequest(request) << body;

	char expectedBody[] = "{\"success\": true}";
	HTTPResponse response;
	
	std::istream& is = cs.receiveResponse(response);
	//cs.receiveResponse(response) >> rbody; // FIXME: this doesn't work and I had to resort to c chars, maybe a bug in POCO

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';

    sql::Driver* driver = get_driver_instance();
    try {
		boost::scoped_ptr<sql::Connection> con(driver->connect("tcp://localhost:3306", "tradingop", "connecttrade"));
		con->setSchema("tradingapp");
		boost::scoped_ptr<sql::PreparedStatement> prep_stmt(
			con->prepareStatement("SELECT id FROM user WHERE name = ?")
		);
		prep_stmt->setString(1, testtrader);
		boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		int id = 0;
		while (res->next()) {
			id = res->getInt("id");
		}
		CPPUNIT_ASSERT(id > 0);
	} catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
    }
	CPPUNIT_ASSERT_EQUAL(response.getContentLength(), (int) strlen(expectedBody));
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

CppUnit::Test* HTTPServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPServerTest");

	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testThrowsErrorOnInvalidRequest",
		&HTTPServerTest::testThrowsErrorOnInvalidRequest
	));
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testSavesTrader",
		&HTTPServerTest::testSavesTrader
	));

	return pSuite;
}

} // namespace: trading
