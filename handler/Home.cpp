
#include "handler/Home.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Path.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <iostream>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void Home::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if(!request.hasCredentials()) 
    { 
        std::string templateFilePath(app.config().getString("application.dir") + "templates/welcome_no_credentials.html");
        response.sendFile(templateFilePath, "text/html");
    }
}

} // namespace trading::handler
} // namespace trading