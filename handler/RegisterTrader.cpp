
#include "handler/RegisterTrader.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/NameValueCollection.h>
#include <iostream>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

RegisterTrader::RegisterTrader(const std::string& format): _format(format) {}

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
        std::cout << "<h2>Form</h2><p>\n";
        std::cout << "username: " << form["username"] << ", pass: " << form["pass"] << "<br>\n";
    }

}

} // namespace trading::handler
} // namespace trading