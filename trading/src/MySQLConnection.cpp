
#include "MySQLConnection.hpp"

namespace trading {

std::unique_ptr<sql::Connection> MySQLConnection::connect() {
    sql::Driver *driver = get_driver_instance();
    Poco::Util::JSONConfiguration config;

    Poco::Util::Application& app = Poco::Util::Application::instance();
    std::ifstream file(app.config().getString("application.dir") + "/../../database_config.json");

    if (file) {
        try {
            config.load(file);
        } catch(const Poco::JSON::JSONException& jsone) {
            app.logger().error(std::string(jsone.message()));
        }
        file.close();
    } else {
        throw "Couldn\'t open configuration file for reading'";
    }
    try {
        auto con = std::unique_ptr<sql::Connection>(driver->connect(config.getString("host"), config.getString("user"), config.getString("password")));
        con->setSchema(config.getString("database"));
        return con;
    }  catch (sql::SQLException &ex) {
        app.logger().error(std::string("# ERR: SQLException with database in ") + __FILE__);
        app.logger().error(std::string("(") + __FUNCTION__ + ") on line " + std::to_string(__LINE__));
        app.logger().error(std::string("# ERR: ") + ex.what());
        app.logger().error(std::string(" (MySQL error code: ") + std::to_string(ex.getErrorCode()));
        throw ex;
    }
}

} // namespace trading
