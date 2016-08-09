
#include "handler/Buy.hpp"
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

void Buy::handleRequest(HTTPServerRequest& request,
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
                std::cout << "tiene los dos" << std::endl;
                bool success = buyStock(form["code"], form["quantity"]);
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

bool Buy::buyStock(std::string const& stockCode, std::string const& quantity) 
{
    try {
        boost::scoped_ptr<sql::Connection> con(trading::MySQLConnection::connect());
        boost::scoped_ptr<sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT id, lastSalePrice FROM stock WHERE code = ?")
        );
        prep_stmt->setString(1, stockCode);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        float lastSalePrice = 0;
        int stockId = 0;
        int userId = 1; // FIXME: obtain user
        while (res->next()) {
            lastSalePrice = res->getDouble("lastSalePrice");
            stockId = res->getInt("id");
        }
        if (stockId == 0) {
            return false;
        }
        boost::scoped_ptr<sql::PreparedStatement> insert_stmt(
            con->prepareStatement("INSERT INTO transaction (userId, stockId, quantity, price, dateOfTransaction, status) VALUES (?, ?, ?, ?, NOW(), 'pending')")
        );
        insert_stmt->setInt(1, userId);
        insert_stmt->setInt(2, stockId);
        insert_stmt->setString(3, quantity);
        insert_stmt->setDouble(4, lastSalePrice);
        insert_stmt->execute();
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
