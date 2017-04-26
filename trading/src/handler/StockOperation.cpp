
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
    app.logger().information(std::string("Request ") + _operation + std::string(" from ")
        + request.clientAddress().toString());

    if (!request.hasCredentials() || !UserAuthentication::isAuthorizedUser(request)) 
    {
        response.redirect("/");
        return;
    }

    Poco::Net::HTMLForm form(request, request.stream());
    if (!form.empty()) {
        if (form.has("code") && form.has("quantity")) {
            Poco::Net::HTTPBasicCredentials cred(request);
            const std::string& user = cred.getUsername(); 
            bool success = operate(form["code"], form["quantity"], user);
            response.setContentType("application/json");
            std::string responseStr(std::string("{\"success\": ") + (success ? "true" : "false") + "}");
            response.sendBuffer(responseStr.data(), responseStr.length());
            return;
        }
    }
    response.setContentType("application/json");
    std::string responseStr("{\"success\": false}");
    response.sendBuffer(responseStr.data(), responseStr.length());
}

bool StockOperation::operate(std::string const& stockCode, std::string const& quantity, std::string const& user) 
{
    try {
        boost::scoped_ptr<sql::Connection> con(trading::MySQLConnection::connect());
        boost::scoped_ptr<sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT id AS userId, balancecash AS balanceCash FROM user WHERE name = ?")
        );
        prep_stmt->setString(1, user);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        int userId = 0;
        float balanceCash = 0;
        while (res->next()) {
            userId = res->getInt("userId");
            balanceCash = res->getDouble("balanceCash");
        }
        if (userId == 0) {
            return false;
        }
        boost::scoped_ptr<sql::PreparedStatement> stock_stmt(
            con->prepareStatement("SELECT stock.id AS stockId, lastSalePrice, COALESCE(portfolio.quantity, 0) AS sharesBought \
                FROM stock \
                LEFT JOIN portfolio ON (portfolio.stockId = stock.id AND portfolio.userId = ?) \
                WHERE code = ?"
            )
        );
        stock_stmt->setInt(1, userId);
        stock_stmt->setString(2, stockCode);
        boost::scoped_ptr<sql::ResultSet> res_stock(stock_stmt->executeQuery());
        float lastSalePrice = 0;
        int stockId = 0;
        int sharesBought = 0;
        int qty = std::stoi(quantity);
        while (res_stock->next()) {
            lastSalePrice = res_stock->getDouble("lastSalePrice");
            stockId = res_stock->getInt("stockId");
            sharesBought = res_stock->getInt("sharesBought");
        }
        if (stockId == 0) {
            return false;
        }
        float costOfOperation = lastSalePrice * qty;
        std::string op = "+";
        std::string balanceOp = "-";
        if (_operation == SELL) {
            op = "-";
            balanceOp = "+";
            if (qty > sharesBought) { // Not allowing to sell shares you don't own
                return false;
            }
        } else if (costOfOperation > balanceCash) {
            return false;
        }
        boost::scoped_ptr<sql::PreparedStatement> insert_stmt(
            con->prepareStatement(std::string("INSERT INTO transaction (userId, stockId, quantity, price, dateOfTransaction, status) VALUES (") +
                "?, ?, ?, ?, NOW(), 'pending')")
        );
        insert_stmt->setInt(1, userId);
        insert_stmt->setInt(2, stockId);
        insert_stmt->setString(3, op + quantity);
        insert_stmt->setDouble(4, lastSalePrice);
        insert_stmt->execute();

        boost::scoped_ptr<sql::PreparedStatement> portfolio_stmt(
            con->prepareStatement(std::string("INSERT INTO portfolio (userId, stockId, quantity, totalCost) VALUES (") + 
                "?, ?, ?, ?) ON DUPLICATE KEY UPDATE quantity=quantity" + op + "?, totalCost=totalCost" + op + "?"
            )
        );
        portfolio_stmt->setInt(1, userId);
        portfolio_stmt->setInt(2, stockId);
        portfolio_stmt->setInt(3, qty);
        portfolio_stmt->setDouble(4, costOfOperation);
        portfolio_stmt->setInt(5, qty);
        portfolio_stmt->setDouble(6, costOfOperation);
        portfolio_stmt->execute();

        boost::scoped_ptr<sql::PreparedStatement> balance_stmt(
            con->prepareStatement(std::string("UPDATE user SET balancecash=balancecash" + balanceOp + "? WHERE id=?"))
        );
        balance_stmt->setDouble(1, costOfOperation);
        balance_stmt->setInt(2, userId);
        balance_stmt->execute();
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
