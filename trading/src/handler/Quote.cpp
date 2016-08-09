
#include "handler/Quote.hpp"
#include "MySQLConnection.hpp"
#include "UserAuthentication.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTMLForm.h>
#include <string>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void Quote::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request from "
        + request.clientAddress().toString());

    if (!request.hasCredentials()) 
    {
        response.redirect("/");
    } //else if (UserAuthentication::isAuthorizedUser(request)) { // FIXME: failing
        Poco::Net::HTMLForm form(request, request.stream());
        if (!form.empty()) {
            if (form.has("stockCode") && stockExists(form["stockCode"])) {
                float quote = getQuote(form["stockCode"]);
                response.setContentType("application/json");
                std::string responseStr("{\"success\": true, \"quote\": " + std::to_string(quote) + "}");
                response.sendBuffer(responseStr.data(), responseStr.length());
            }
        }
        response.setContentType("application/json");
        std::string responseStr("{\"success\": false}");
        response.sendBuffer(responseStr.data(), responseStr.length());
    // }
}

bool Quote::stockExists(std::string stockCode) 
{
    try {
        boost::scoped_ptr<sql::Connection> con(trading::MySQLConnection::connect());
        boost::scoped_ptr< sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT id FROM stock WHERE code = ?")
        );
        prep_stmt->setString(1, stockCode);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        int id = 0;
        while (res->next()) {
            id = res->getInt("id");
        }
        return id > 0;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        exit(1);
    }
    return false;
}

float Quote::getQuote(std::string stockCode) 
{
    try {
        boost::scoped_ptr<sql::Connection> con(trading::MySQLConnection::connect());
        boost::scoped_ptr< sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT lastSalePrice FROM stock WHERE code = ?")
        );
        prep_stmt->setString(1, stockCode);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        float lastSalePrice = 0;
        while (res->next()) {
            lastSalePrice = res->getDouble("lastSalePrice");
        }
        return lastSalePrice;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        exit(1);
    }
    return 0;
}


} // namespace trading::handler
} // namespace trading
