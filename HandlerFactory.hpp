#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

namespace trading {

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;

class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    HandlerFactory(const std::string& format);

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

private:
    std::string _format;

};

} // namespace trading