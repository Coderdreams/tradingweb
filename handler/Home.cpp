
#include "handler/Home.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Path.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <iostream>
#include <fstream>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;
using Poco::Timestamp;
using Poco::DateTimeFormatter;

Home::Home(const std::string& format): _format(format) {}

void Home::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if(!request.hasCredentials()) 
    { 
        //std::ifstream welcomeTemplate("");
        std::string templateFilePath(app.config().getString("application.dir") + "templates/welcome_no_credentials.html");
        response.sendFile(templateFilePath, "text/html");
        /*
        response.requireAuthentication("My Realm");
        response.setContentLength(0);
        response.send();
        */ 
        return; 
    }

    Timestamp now;
    std::string dt(DateTimeFormatter::format(now, _format));

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/html");

    std::ostream& ostr = response.send();
    ostr << "<html><head><title>HTTPTimeServer powered by "
            "POCO C++ Libraries</title>";
    ostr << "<meta http-equiv=\"refresh\" content=\"1\"></head>";
    ostr << "<body><p style=\"text-align: center; "
            "font-size: 48px;\">";
    ostr << dt;
    ostr << "</p></body></html>";
}

} // namespace trading::handler
} // namespace trading