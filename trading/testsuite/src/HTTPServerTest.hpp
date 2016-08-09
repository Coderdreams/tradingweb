
#pragma once

#include <Poco/Net/Net.h>
#include <cppunit/TestCase.h>
#include "Poco/Process.h"
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
	std::unique_ptr<Poco::ProcessHandle> _ph;
};

} // namespace: trading