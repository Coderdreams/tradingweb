#pragma once

#include <Poco/Net/HTTPServer.h>

namespace trading {

class HTTPServer
{
public:
    void start(unsigned short port);
    void stop();

private:
    Poco::Net::HTTPServer* _srv;
};

} // namespace trading