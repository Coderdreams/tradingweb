
#include "handler/RegisterTrader.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/StreamCopier.h>
#include <iostream>
#include <sstream>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void RegisterTrader::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if (request.hasCredentials()) {
        app.logger().information("User already registered");
        response.redirect("/");
        return;
    }

    Poco::Net::HTMLForm form(request, request.stream());
    if (!form.empty()) {
        if (form.has("username") && form.has("pass")) {
            response.setContentType("application/json");
            std::string responseStr("{\"success\": true}");
            response.sendBuffer(responseStr.data(), responseStr.length());
        } else {
            response.setContentType("application/json");
            std::string responseStr("{\"success\": false}");
            response.sendBuffer(responseStr.data(), responseStr.length());
        }
    }

}

} // namespace trading::handler
} // namespace trading