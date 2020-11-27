
#include "MySQLConnection.hpp"
#include "RegisterTrader.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/NameValueCollection.h>
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
    app.logger().information("Request register from "
        + request.clientAddress().toString());

    if (request.hasCredentials()) {
        app.logger().information("User already registered");
        response.redirect("/");
        return;
    }
    app.logger().information("Attemtp to read request");
    try {
        Poco::Net::HTMLForm form(request, request.stream());

        if (!form.empty()) {
            if (form.has("username") && form.has("pass")) {
                saveUser(form["username"], form["pass"]);
                response.setContentType("application/json");
                std::string responseStr("{\"success\": true}");
                response.sendBuffer(responseStr.data(), responseStr.length());
            }
            return;
        }

    } catch (const Poco::Net::HTMLFormException& ex) {
        app.logger().error(std::string("Couldn't parse html form") + ex.what());
    } catch (const std::exception& ex) {
        app.logger().error(std::string("whatever") + ex.what());
        throw ex;
    }
    response.setContentType("application/json");
    std::string responseStr("{\"success\": false}");
    response.sendBuffer(responseStr.data(), responseStr.length());
}

void RegisterTrader::saveUser(std::string username, std::string password)
{
    Application& app = Application::instance();
    try {
        auto con = trading::MySQLConnection::connect();
        // FIXME: for security hash the pass before insert
        std::unique_ptr<sql::PreparedStatement> prep_stmt(
            con->prepareStatement("INSERT IGNORE INTO user(name, pass) VALUES (?, SHA2(?, 512))")
        );
        prep_stmt->setString(1, username);
        prep_stmt->setString(2, password);
        prep_stmt->execute();
    } catch (const sql::SQLException &ex) {
        app.logger().error(std::string("# ERR: SQLException with database in ") + __FILE__);
        app.logger().error(std::string("(") + __FUNCTION__ + ") on line " + std::to_string(__LINE__));
        app.logger().error(std::string("# ERR: ") + ex.what());
        app.logger().error(std::string(" (MySQL error code: ") + std::to_string(ex.getErrorCode()));
        throw ex;
    }

}


} // namespace trading::handler
} // namespace trading