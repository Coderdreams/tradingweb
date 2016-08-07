
#include "handler/SignUp.hpp"

#include <Poco/Util/Application.h>
#include <string>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void SignUp::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if (request.hasCredentials()) 
    {
        app.logger().information("User already registered");
        response.redirect("/");
        return;
    }

    std::string templateFilePath(app.config().getString("application.dir") + "templates/signup.html");
    response.sendFile(templateFilePath, "text/html");
}

} // namespace trading::handler
} // namespace trading