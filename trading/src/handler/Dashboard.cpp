#include "handler/Dashboard.hpp"

#include <Poco/Util/Application.h>
#include <string>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void Dashboard::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request dashboard from "
        + request.clientAddress().toString());

    if (!request.hasCredentials()) 
    {
        app.logger().information("Can\t access.");
        response.redirect("/");
        return;
    }

    std::string templateFilePath(app.config().getString("application.dir") + "templates/dashboard.html");
    response.sendFile(templateFilePath, "text/html");
}

} // namespace trading::handler
} // namespace trading