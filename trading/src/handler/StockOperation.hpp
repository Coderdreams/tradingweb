#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

namespace trading {
namespace handler {

class StockOperation : public Poco::Net::HTTPRequestHandler
{
public:
    static const std::string BUY;
    static const std::string SELL;
    StockOperation(std::string const& operation);
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

private:
	bool operate(std::string const& stockCode, std::string const& quantity, std::string const& user);
	std::string const& _operation;
};

} // namespace trading::handler
} // namespace trading