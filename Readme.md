# Dependencies

Ubuntu 16.04 Xenial

sudo apt-get install g++ libpoco-dev libcppunit-dev mysql-server libmysqlcppconn-dev

# Create a mysql user for the app with these commands

$ mysql -uroot -p # credentials are specific to your server 

CREATE USER 'tradingop'@'localhost' IDENTIFIED BY 'connecttrade';
GRANT ALL PRIVILEGES ON tradingapp.* TO 'tradingop'@'localhost' WITH GRANT OPTION;
CREATE DATABASE tradingapp;

# Populate the database with the starting script

$ mysql -utradinop -pconnecttrade < database/schema.sql
 