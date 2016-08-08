
#include "handler/RegisterTrader.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/StreamCopier.h>
#include <iostream>
#include <sstream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

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
            saveUser(form["username"], form["pass"]);
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

void RegisterTrader::saveUser(std::string username, std::string password) 
{
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();

    try {
        con = driver->connect("tcp://localhost:3306", "tradingop", "connecttrade");
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        exit(1);
    }
    con->setSchema("tradingapp");

    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
    while (res->next()) {
        std::cout << "\t... MySQL replies: ";

        std::cout << res->getString("_message") << std::endl;
        std::cout << "\t... MySQL says it again: ";

        std::cout << res->getString(1) << std::endl;
    }
    delete res;
    delete stmt;
    delete con;
}


} // namespace trading::handler
} // namespace trading