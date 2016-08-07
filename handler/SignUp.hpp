#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

namespace trading {
namespace handler {

class SignUp : public Poco::Net::HTTPRequestHandler
{
public:

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);
};

} // namespace trading::handler
} // namespace trading