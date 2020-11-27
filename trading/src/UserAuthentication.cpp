
#include "UserAuthentication.hpp"
#include "MySQLConnection.hpp"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Util/Application.h>

#include <string>

namespace trading {

using Poco::Util::Application;

bool UserAuthentication::isAuthorizedUser(Poco::Net::HTTPServerRequest& request)
{
        Application& app = Application::instance();
        Poco::Net::HTTPBasicCredentials cred(request);
        const std::string& user = cred.getUsername();
        const std::string& pwd = cred.getPassword();

        auto con = trading::MySQLConnection::connect();
        try {

        std::unique_ptr<sql::PreparedStatement> prep_stmt(
                con->prepareStatement("SELECT id FROM user WHERE name = ? AND pass = SHA2(?, 512)"       )
        );

                prep_stmt->setString(1, user);
        prep_stmt->setString(2, pwd);
        std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        int id = 0;
        while (res->next()) {
                id = res->getInt("id");
        }
        if (id > 0) {
                return true;
        } else {
                return false;
        }
        } catch( const sql::SQLException & ex ) {
                app.logger().error(std::string("# ERR: SQLException with database in ") + __FILE__);
                app.logger().error(std::string("(") + __FUNCTION__ + ") on line " + std::to_string(__LINE__));
                app.logger().error(std::string("# ERR: ") + ex.what());
                app.logger().error(std::string(" (MySQL error code: ") + std::to_string(ex.getErrorCode()));
                throw ex;
        } catch (const std::exception &e) {
                app.logger().error(std::string("# WTF: ") + e.what());
                throw e;
        }

}

} // namespace trading