# Dependencies

Ubuntu 16.04 Xenial 64bits

Tested with gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.1) (check with g++ -v)

$ sudo apt-get install g++ libcppunit-dev mysql-server libmysqlcppconn-dev

Download and uncompress poco from here: http://pocoproject.org/releases/poco-1.7.4/poco-1.7.4.tar.gz

# This is a slow step, but the poco libraries on Ubuntu are too old and missing lots of features

$ cd poco-1.7.4
$ ./configure
$ make
$ sudo make install
# This one is to refresh the library config database for the new installed libraries
$ sudo ldconfig

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

