
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
#include <cppconn/prepared_statement.h>

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

    try {
        driver = get_driver_instance();
        boost::scoped_ptr<sql::Connection> con(driver->connect("tcp://localhost:3306", "tradingop", "connecttrade"));
        con->setSchema("tradingapp");

        boost::scoped_ptr< sql::PreparedStatement> prep_stmt(
            con->prepareStatement("INSERT INTO user(name, pass) VALUES (?, PASSWORD(?))")
        );
        prep_stmt->setString(1, username);
        prep_stmt->setString(2, password);
        prep_stmt->execute();
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        exit(1);
    }

}


} // namespace trading::handler
} // namespace trading