
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/JSON/JSONException.h"
#include <Poco/Util/Application.h>
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <fstream>

namespace trading {

class MySQLConnection {
    public:
    static sql::Connection* connect() {
        sql::Driver *driver = get_driver_instance();
        Poco::Util::JSONConfiguration config;

        Poco::Util::Application& app = Poco::Util::Application::instance();
        std::ifstream file(app.config().getString("application.dir") + "/database_config.json");

        if (file) {
            try {
                config.load(file);
            } catch(Poco::JSON::JSONException jsone) {
                std::cout << jsone.message() << std::endl;
            }
            file.close();
        } else {
            throw "Couldn\'t open configuration file for reading'";
        }
        try {
            sql::Connection* con = driver->connect(config.getString("host"), config.getString("user"), config.getString("password"));
            con->setSchema(config.getString("database"));
            return con;
        }  catch (sql::SQLException &e) {
            std::cout << "# ERR: SQLException connecting to database in " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
            std::cout << "# ERR: " << e.what();
            std::cout << " (MySQL error code: " << e.getErrorCode();
            throw e;
        }
    }
};

} // namespace trading