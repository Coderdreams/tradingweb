
#include "UserAuthentication.hpp"
#include "MySQLConnection.hpp"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <string>

namespace trading {

bool UserAuthentication::isAuthorizedUser(Poco::Net::HTTPServerRequest& request)
{
        Poco::Net::HTTPBasicCredentials cred(request);
        const std::string& user = cred.getUsername(); 
        const std::string& pwd = cred.getPassword();

        auto con = trading::MySQLConnection::connect();
        boost::scoped_ptr<sql::PreparedStatement> prep_stmt(
                con->prepareStatement("SELECT id FROM user WHERE name = ? AND pass = PASSWORD(?)")
        );
        prep_stmt->setString(1, user);
        prep_stmt->setString(2, pwd);
        boost::scoped_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
        int id = 0;
        while (res->next()) {
                id = res->getInt("id");
        }
        if (id > 0) {
                return true;
        } else {
                return false;
        }
}

} // namespace trading