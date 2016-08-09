echo "DROP DATABASE tradingapp; CREATE DATABASE tradingapp;" | mysql -utradingop -pconnecttrade 
mysql -utradingop -pconnecttrade tradingapp < database/schema.sql
./get_stock_data.sh | mysql -utradingop -pconnecttrade tradingapp