
#pragma once

#include <Poco/Net/Net.h>
#include <cppunit/TestCase.h>

namespace trading {

class HTTPServerTest : public CppUnit::TestCase
{
public:

	void testIdentityRequest();
	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};

} // namespace: trading