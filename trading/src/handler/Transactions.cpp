
#include "Transactions.hpp"
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
    app.logger().information("Request transactions from "
        + request.clientAddress().toString());

    if (!request.hasCredentials() || !UserAuthentication::isAuthorizedUser(request))
    {
        response.redirect("/");
        return;
    }

    Poco::Net::HTTPBasicCredentials cred(request);
    const std::string& user = cred.getUsername();
    std::string jsonString(get(user));
    response.setContentType("application/json");
    std::string responseStr("{\"success\": true, \"results\": " + jsonString + "}");
    response.sendBuffer(responseStr.data(), responseStr.length());
}

std::string Transactions::get(std::string const& user)
{
    Application& app = Application::instance();
    try {
        auto con = trading::MySQLConnection::connect();
        std::unique_ptr<sql::PreparedStatement> prep_stmt(
            con->prepareStatement(std::string("SELECT stock.code as stockCode, quantity, FORMAT(price*quantity, 2) AS price, dateOfTransaction, status FROM transaction ") +
                " LEFT JOIN user ON (transaction.userId = user.id) " +
                " LEFT JOIN stock ON (transaction.stockId = stock.id) " +
                " WHERE user.name = ?"
            )
        );
        prep_stmt->setString(1, user);
        std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
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
    } catch (const sql::SQLException &ex) {
            app.logger().error(std::string("# ERR: SQLException with database in ") + __FILE__);
            app.logger().error(std::string("(") + __FUNCTION__ + ") on line " + std::to_string(__LINE__));
            app.logger().error(std::string("# ERR: ") + ex.what());
            app.logger().error(std::string(" (MySQL error code: ") + std::to_string(ex.getErrorCode()));
        throw ex;
    }
    return "[]";
}

} // namespace trading::handler
} // namespace trading
