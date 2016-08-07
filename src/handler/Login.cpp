
#include "handler/Login.hpp"

#include <Poco/Util/Application.h>
#include <iostream>
#include <fstream>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void Login::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if (!request.hasCredentials()) 
    {
        response.requireAuthentication("TradingApp");
        response.setContentLength(0);
        response.send();
    }
}

} // namespace trading::handler
} // namespace trading