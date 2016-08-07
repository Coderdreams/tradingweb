#pragma once

#include <Poco/Net/HTTPServer.h>
#include <memory>

namespace trading {

class HTTPServer
{
public:
    void start(unsigned short port);
    void stop();

private:
    std::unique_ptr<Poco::Net::HTTPServer> _srv;
};

} // namespace trading