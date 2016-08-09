
#include "handler/StockOperation.hpp"
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

const std::string StockOperation::BUY   = "buy";
const std::string StockOperation::SELL  = "sell";

StockOperation::StockOperation(std::string const& operation) : _operation(operation) {}

void StockOperation::handleRequest(HTTPServerRequest& request,
                    HTTPServerResponse& response)
{
    Application& app = Application::instance();
    app.logger().information("Request buy from "
        + request.clientAddress().toString());

    if (!request.hasCredentials()) 
    {
        response.redirect("/");
    } //else if (UserAuthentication::isAuthorizedUser(request)) { // FIXME: failing
        Poco::Net::HTMLForm form(request, request.stream());
        if (!form.empty()) {
                std::cout << "empty?" << std::endl;
            if (form.has("code") && form.has("quantity")) {
                Poco::Net::HTTPBasicCredentials cred(request);
                const std::string& user = cred.getUsername(); 
                bool success = operate(form["code"], form["quantity"], user);
                response.setContentType("application/json");
                std::string responseStr("{\"success\": " + std::to_string(success) + "}");
                response.sendBuffer(responseStr.data(), responseStr.length());
                return;
            }
        }
        response.setContentType("application/json");
        std::string responseStr("{\"success\": false}");
        response.sendBuffer(responseStr.data(), responseStr.length());
    // }
}

bool StockOperation::operate(std::string const& stockCode, std::string const& quantity, std::string const& user) 
{
    try {
        boost::scoped_ptr<sql::Connection> con(trading::MySQLConnection::connect());
        boost::scoped_ptr<sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT id, lastSalePrice, (SELECT user.id FROM user WHERE name = ?) AS userId FROM stock WHERE code = ?")
        );
        prep_stmt->setString(1, user);
        prep_stmt->setString(2, stockCode);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        float lastSalePrice = 0;
        int stockId = 0;
        int userId = 0;
        while (res->next()) {
            lastSalePrice = res->getDouble("lastSalePrice");
            stockId = res->getInt("id");
            userId = res->getInt("userId");
        }
        if (stockId == 0) {
            return false;
        }
        boost::scoped_ptr<sql::PreparedStatement> insert_stmt(
            con->prepareStatement(std::string("INSERT INTO transaction (userId, stockId, quantity, price, dateOfTransaction, status) VALUES (") +
                "?, ?, ?, ?, NOW(), 'pending')")
        );
        insert_stmt->setInt(1, userId);
        insert_stmt->setInt(2, stockId);
        insert_stmt->setString(3, quantity);
        insert_stmt->setDouble(4, lastSalePrice);
        insert_stmt->execute();

        std::string op = "+";
        if (_operation == SELL) {
            op = "-";
        }
        boost::scoped_ptr<sql::PreparedStatement> portfolio_stmt(
            con->prepareStatement(std::string("INSERT INTO portfolio (userId, stockId, quantity, totalCost) VALUES (") + 
                "?, ?, ?, ?) ON DUPLICATE KEY UPDATE quantity=quantity" + op + "?, totalCost=totalCost" + op + "?"
            )
        );
        int qty = std::stoi(quantity);
        float costOfOperation = lastSalePrice * qty;
        portfolio_stmt->setInt(1, userId);
        portfolio_stmt->setInt(2, stockId);
        portfolio_stmt->setInt(3, qty);
        portfolio_stmt->setDouble(4, costOfOperation);
        portfolio_stmt->setInt(5, qty);
        portfolio_stmt->setDouble(6, costOfOperation);
        portfolio_stmt->execute();
        return true;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        exit(1);
    }
    return false;
}

} // namespace trading::handler
} // namespace trading
