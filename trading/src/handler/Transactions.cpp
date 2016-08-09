
#include "handler/Transactions.hpp"
#include "MySQLConnection.hpp"
#include "UserAuthentication.hpp"

#include <Poco/Util/Application.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <string>

namespace trading {
namespace handler {

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

void Transactions::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request buy from "
        + request.clientAddress().toString());

    if (!request.hasCredentials()) 
    {
        response.redirect("/");
    } //else if (UserAuthentication::isAuthorizedUser(request)) { // FIXME: failing
        Poco::Net::HTTPBasicCredentials cred(request);
        const std::string& user = cred.getUsername(); 
        std::string jsonString(get(user));
        response.setContentType("application/json");
        std::string responseStr("{\"success\": true, \"results\": " + jsonString + "}");
        response.sendBuffer(responseStr.data(), responseStr.length());
        return;
    // }
}

std::string Transactions::get(std::string const& user) 
{
    try {
        boost::scoped_ptr<sql::Connection> con(trading::MySQLConnection::connect());
        boost::scoped_ptr<sql::PreparedStatement> prep_stmt(
            con->prepareStatement(std::string("SELECT stock.code as stockCode, quantity, price, dateOfTransaction, status FROM transaction ") +
                " LEFT JOIN user ON (transaction.userId = user.id) " +
                " LEFT JOIN stock ON (transaction.stockId = stock.id) " +
                " WHERE user.name = ?"
            )
        );
        prep_stmt->setString(1, user);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        std::string strJson;
        while (res->next()) {
            strJson += std::string("{\"stock_code\": \"") + res->getString("stockCode") + "\",";
            strJson += std::string("\"quantity\": \"") + res->getString("quantity") + "\",";
            strJson += std::string("\"price\": \"") + res->getString("price") + "\",";
            strJson += std::string("\"date\": \"") + res->getString("dateOfTransaction") + "\",";
            strJson += std::string("\"status\": \"") + res->getString("status") + "\"}";
            if (!res->isLast()) {
                strJson += ",";
            }
        }
        return std::string("[" + strJson + "]");
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        exit(1);
    }
    return "[]";
}

} // namespace trading::handler
} // namespace trading
