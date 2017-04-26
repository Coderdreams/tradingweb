# Dependencies

Ubuntu 16.04 Xenial 64bit

Tested with gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.1) (check with g++ -v)

$ sudo apt-get install g++ mysql-server libmysqlcppconn-dev premake4

# Download and install conan package manager:

https://github.com/conan-io/conan/releases/download/0.22.2/conan-ubuntu-64_0_22_2.deb

# Build with script:

$ ./build.sh

# Activate the conan deps for the app to find its compiled .so's at runtime

$ source activate_run.sh

# Create a mysql user for the app with these commands

$ mysql -uroot -p # credentials are specific to your server 

# On mysql's prompt copy and paste these:

> CREATE USER 'tradingop'@'localhost' IDENTIFIED BY 'connecttrade';
> GRANT ALL PRIVILEGES ON tradingapp.* TO 'tradingop'@'localhost' WITH GRANT OPTION;
> CREATE DATABASE tradingapp;

If the credentials need to be different change them here, and edit database_config.json to match them.

# Populate the database with the starting script

$ mysql -utradingop -pconnecttrade tradingapp < database/schema.sql
$ ./get_stock_data.sh | mysql -utradingop -pconnecttrade tradingapp
 
# To run tests (BEWARE, they DROP the database for cleanup)

$ ./run_tests.sh

# To compile

$ make

# To execute the application

$ ./tradingApp

# Point your browser to http://localhost:9980
