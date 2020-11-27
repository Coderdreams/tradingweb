
#include "Quote.hpp"
#include "MySQLConnection.hpp"

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
    app.logger().information("Request quote from "
        + request.clientAddress().toString());

    Poco::Net::HTMLForm form(request, request.stream());
    if (!form.empty()) {
        if (form.has("stockCode") && stockExists(form["stockCode"])) {
            float quote = getQuote(form["stockCode"]);
            response.setContentType("application/json");
            std::string responseStr("{\"success\": true, \"quote\": " + std::to_string(quote) + "}");
            response.sendBuffer(responseStr.data(), responseStr.length());
            return;
        }
    }
    response.setContentType("application/json");
    std::string responseStr("{\"success\": false}");
    response.sendBuffer(responseStr.data(), responseStr.length());
}

bool Quote::stockExists(std::string stockCode)
{
    Application& app = Application::instance();
    try {
        auto con = trading::MySQLConnection::connect();
        std::unique_ptr< sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT id FROM stock WHERE code = ?")
        );
        prep_stmt->setString(1, stockCode);
        std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        int id = 0;
        while (res->next()) {
            id = res->getInt("id");
        }
        return id > 0;
    } catch (const sql::SQLException &ex) {
        app.logger().error(std::string("# ERR: SQLException with database in ") + __FILE__);
        app.logger().error(std::string("(") + __FUNCTION__ + ") on line " + std::to_string(__LINE__));
        app.logger().error(std::string("# ERR: ") + ex.what());
        app.logger().error(std::string(" (MySQL error code: ") + std::to_string(ex.getErrorCode()));
        throw ex;
    }
    return false;
}

float Quote::getQuote(std::string stockCode)
{
    Application& app = Application::instance();
    try {
        auto con = trading::MySQLConnection::connect();
        std::unique_ptr< sql::PreparedStatement> prep_stmt(
            con->prepareStatement("SELECT lastSalePrice FROM stock WHERE code = ?")
        );
        prep_stmt->setString(1, stockCode);
        std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        float lastSalePrice = 0;
        while (res->next()) {
            lastSalePrice = res->getDouble("lastSalePrice");
        }
        return lastSalePrice;
    } catch (const sql::SQLException &ex) {
        app.logger().error(std::string("# ERR: SQLException with database in ") + __FILE__);
        app.logger().error(std::string("(") + __FUNCTION__ + ") on line " + std::to_string(__LINE__));
        app.logger().error(std::string("# ERR: ") + ex.what());
        app.logger().error(std::string(" (MySQL error code: ") + std::to_string(ex.getErrorCode()));
        throw ex;
    }
    return 0;
}


} // namespace trading::handler
} // namespace trading
