#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

namespace trading {
namespace handler {

class Quote : public Poco::Net::HTTPRequestHandler
{
public:

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

private:
	bool stockExists(std::string stockCode);
	float getQuote(std::string stockCode);
};

} // namespace trading::handler
} // namespace trading