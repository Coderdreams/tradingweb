#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

namespace trading {
namespace handler {

class Login : public Poco::Net::HTTPRequestHandler
{
public:
    Login(const std::string& format);

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

private:
    std::string _format;
};

} // namespace trading::handler
} // namespace trading