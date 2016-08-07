#include "HandlerFactory.hpp"
#include "handler/Home.hpp"

namespace trading {

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

HandlerFactory::HandlerFactory(const std::string& format):
    _format(format)
{
}

HTTPRequestHandler* HandlerFactory::createRequestHandler(
    const HTTPServerRequest& request)
{
    if (request.getURI() == "/")
        return new trading::handler::Home(_format);
    else if (request.getURI() == "/login")
        return new trading::handler::Login(_format);
    else
        return 0;
}

} // namespace trading