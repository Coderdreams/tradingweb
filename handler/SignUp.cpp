
#include "handler/SignUp.hpp"

#include <Poco/Util/Application.h>
#include <iostream>
#include <fstream>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

SignUp::SignUp(const std::string& format): _format(format) {}

void SignUp::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if (!request.hasCredentials()) 
    {
        std::string templateFilePath(app.config().getString("application.dir") + "templates/signup.html");
        response.sendFile(templateFilePath, "text/html");
    }
}

} // namespace trading::handler
} // namespace trading