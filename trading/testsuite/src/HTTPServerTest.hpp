
#pragma once

#include <Poco/Net/Net.h>
#include <cppunit/TestCase.h>
#include <memory>

namespace trading {

class HTTPServerTest : public CppUnit::TestCase
{
public:
	HTTPServerTest();
	~HTTPServerTest();
	void testThrowsErrorOnInvalidRequest();
	void testSavesTrader();
	void testGetQuote();
	void testBuyUnauthorized();
	void testBuy();
	void testSell();
	void testPortfolio();
	void testTransactions();
	void setUp();
	void tearDown();
	static CppUnit::Test* suite();
private:
	bool assertJsonValuesAreEqual(std::string, std::string);

};

} // namespace: trading