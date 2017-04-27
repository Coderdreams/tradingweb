
#include "MySQLConnection.hpp"

namespace trading {

sql::Connection* MySQLConnection::connect() {
    if (!con) {
        _connect();
    }
    return con.get();
}

void MySQLConnection::_connect() {
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
        con = std::unique_ptr<sql::Connection>(driver->connect(config.getString("host"), config.getString("user"), config.getString("password")));
        con->setSchema(config.getString("database"));
    }  catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException connecting to database in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        throw e;
    }
}

std::unique_ptr<sql::Connection> MySQLConnection::con;

} // namespace trading
