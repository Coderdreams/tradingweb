#pragma once

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
    static sql::Connection* con;
    static void _connect();

public:
    ~MySQLConnection();
    static sql::Connection* connect();

};

} // namespace trading
