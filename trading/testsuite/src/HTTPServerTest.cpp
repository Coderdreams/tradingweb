
#include "HTTPServerTest.hpp"
#include "MySQLConnection.hpp"

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/LoggingSubsystem.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/StreamCopier.h>
#include <string>
#include <iostream>
#include <unistd.h>


namespace trading {

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Util::Application;
using Poco::Logger;
using namespace std::string_literals;

HTTPServerTest::HTTPServerTest() {}

HTTPServerTest::~HTTPServerTest() {}

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

	CPPUNIT_ASSERT(response.getContentLength() == (int) strlen(expectedBody));
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

void HTTPServerTest::testSavesTrader()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	std::string testtrader("testtrader");

	HTTPRequest request(HTTPRequest::HTTP_POST, "/registerTrader");

	Poco::Net::HTMLForm form;
	form.add("username", "testtrader"); 
	form.add("pass", "test123");
	form.prepareSubmit(request); 
	request.setContentType("application/json");
	form.write(cs.sendRequest(request));

	char expectedBody[] = "{\"success\": true}";
	HTTPResponse response;
	
	std::istream& is = cs.receiveResponse(response);

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';

	auto con = trading::MySQLConnection::connect();
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
	int expectedBodyLength = (int) strlen(expectedBody);
	int responseLength = (int) response.getContentLength();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(rbody, responseLength, expectedBodyLength);
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

void HTTPServerTest::testGetQuote()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/quote");

	Poco::Net::HTMLForm form;
	form.add("stockCode", "TXT"); 
	form.prepareSubmit(request); 
	request.setContentType("application/json");
	form.write(cs.sendRequest(request));

	char expectedBody[] = "{\"success\": true, \"quote\": 57.939999}";
	HTTPResponse response;
	
	std::istream& is = cs.receiveResponse(response);

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';

	int expectedBodyLength = (int) strlen(expectedBody);
	int responseLength = (int) response.getContentLength();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(rbody, responseLength, expectedBodyLength);
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

void HTTPServerTest::testBuyUnauthorized()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/buy");
	std::string testtrader("testtrader");

	Poco::Net::HTMLForm form;
	form.add("code", "TXT");
	form.add("quantity", "10");
	form.prepareSubmit(request); 
	request.setContentType("application/json");
	form.write(cs.sendRequest(request));

	HTTPResponse response;

	cs.receiveResponse(response);

	int expectedBodyLength = 0;
	int responseLength = (int) response.getContentLength();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("No response should be received", expectedBodyLength, responseLength);
}

void HTTPServerTest::testBuy()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/buy");
	std::string testtrader("testtrader");
    Poco::Net::HTTPBasicCredentials cred(testtrader, "test123");
    cred.authenticate(request);

	Poco::Net::HTMLForm form;
	form.add("code", "TXT");
	form.add("quantity", "10");
	form.prepareSubmit(request); 
	request.setContentType("application/json");
	form.write(cs.sendRequest(request));

	char expectedBody[] = "{\"success\": true}";
	HTTPResponse response;
	
	std::istream& is = cs.receiveResponse(response);

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';
	
	int expectedBodyLength = (int) strlen(expectedBody);
	int responseLength = (int) response.getContentLength();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(rbody, responseLength, expectedBodyLength);
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

void HTTPServerTest::testSell()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/sell");
	std::string testtrader("testtrader");
    Poco::Net::HTTPBasicCredentials cred(testtrader, "test123");
    cred.authenticate(request);

	Poco::Net::HTMLForm form;
	form.add("code", "TXT");
	form.add("quantity", "2");
	form.prepareSubmit(request); 
	request.setContentType("application/json");
	form.write(cs.sendRequest(request));

	char expectedBody[] = "{\"success\": true}";
	HTTPResponse response;
	
	std::istream& is = cs.receiveResponse(response);

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';
	
	int expectedBodyLength = (int) strlen(expectedBody);
	int responseLength = (int) response.getContentLength();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(rbody, responseLength, expectedBodyLength);
	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
	CPPUNIT_ASSERT_EQUAL(strcmp(expectedBody, rbody), 0);
}

void HTTPServerTest::testTransactions()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/transactions");
	std::string testtrader("testtrader");
    Poco::Net::HTTPBasicCredentials cred(testtrader, "test123");
    cred.authenticate(request);

	cs.sendRequest(request);

	HTTPResponse response;

	std::istream& is = cs.receiveResponse(response);

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var result = parser.parse(rbody);
	Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var success = object->get("success");
	std::string value = success.convert<std::string>();
	CPPUNIT_ASSERT_EQUAL(value, std::string("true"));
	
	Poco::Dynamic::Var results = object->get("results");
	Poco::JSON::Array::Ptr arr = results.extract<Poco::JSON::Array::Ptr>();
	CPPUNIT_ASSERT_EQUAL((int) arr->size(), 2);

	Poco::JSON::Object::Ptr result1 = arr->getObject(0);
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("stock_code"), std::string("TXT"));
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("quantity"), std::string("10"));
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("price"), std::string("579.40"));
	CPPUNIT_ASSERT(result1->getValue<std::string>("date") != ""); // FIXME: verify it's a correct date
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("status"), std::string("pending"));

	Poco::JSON::Object::Ptr result2 = arr->getObject(1);
	CPPUNIT_ASSERT_EQUAL(result2->getValue<std::string>("stock_code"), std::string("TXT"));
	CPPUNIT_ASSERT_EQUAL(result2->getValue<std::string>("quantity"), std::string("-2"));
	CPPUNIT_ASSERT_EQUAL(result2->getValue<std::string>("price"), std::string("-115.88"));
	CPPUNIT_ASSERT(result2->getValue<std::string>("date") != ""); // FIXME: verify it's a correct date
	CPPUNIT_ASSERT_EQUAL(result2->getValue<std::string>("status"), std::string("pending"));

	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);

}

void HTTPServerTest::testPortfolio()
{
	unsigned short port = 9980;

	HTTPClientSession cs("localhost", port);
	HTTPRequest request(HTTPRequest::HTTP_POST, "/portfolio");
	std::string testtrader("testtrader");
    Poco::Net::HTTPBasicCredentials cred(testtrader, "test123");
    cred.authenticate(request);

	cs.sendRequest(request);

	HTTPResponse response;

	std::istream& is = cs.receiveResponse(response);

	unsigned int contentLength = response.getContentLength();
	char rbody[500];
	is.read(rbody, contentLength);
	rbody[contentLength] = '\0';

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var result = parser.parse(rbody);
	Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
	Poco::Dynamic::Var success = object->get("success");
	std::string value = success.convert<std::string>();
	CPPUNIT_ASSERT_EQUAL(value, std::string("true"));
	
	Poco::Dynamic::Var results = object->get("results");
	Poco::JSON::Array::Ptr arr = results.extract<Poco::JSON::Array::Ptr>();
	CPPUNIT_ASSERT_EQUAL((int) arr->size(), 1);

	Poco::JSON::Object::Ptr result1 = arr->getObject(0);
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("stock_code"), std::string("TXT"));
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("quantity"), std::string("8"));
	CPPUNIT_ASSERT_EQUAL(result1->getValue<std::string>("cost"), std::string("463.52"));

	CPPUNIT_ASSERT_EQUAL(response.getContentType(), "application/json"s);
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
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testGetQuote",
		&HTTPServerTest::testGetQuote
	));
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testBuyUnauthorized",
		&HTTPServerTest::testBuyUnauthorized
	));
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testBuy",
		&HTTPServerTest::testBuy
	));
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testSell",
		&HTTPServerTest::testSell
	));
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testTransactions",
		&HTTPServerTest::testTransactions
	));
	pSuite->addTest(new CppUnit::TestCaller<HTTPServerTest>(
		"testPortfolio",
		&HTTPServerTest::testPortfolio
	));
	return pSuite;
}

} // namespace: trading
