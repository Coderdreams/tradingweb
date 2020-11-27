
#include "StaticContent.hpp"

#include <Poco/Util/Application.h>
#include <string>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void StaticContent::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request static content from "
        + request.clientAddress().toString());
    std::string uri(request.getURI());
    std::string staticFolder("static/");
    std::string templateFilePath(app.config().getString("application.dir") + "../../" + staticFolder +
        uri.substr(staticFolder.length()));
    response.sendFile(templateFilePath, "text/html");
}

} // namespace trading::handler
} // namespace trading