target_sources(tradingApp PRIVATE
    Dashboard.cpp
    Home.cpp
    Login.cpp
    Portfolio.cpp
    Quote.cpp
    RegisterTrader.cpp
    SignUp.cpp
    StaticContent.cpp
    StockOperation.cpp
    Transactions.cpp
)
target_include_directories(tradingApp PUBLIC ${CMAKE_CURRENT_LIST_DIR})
