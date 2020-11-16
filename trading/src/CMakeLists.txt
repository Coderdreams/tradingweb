target_sources(tradingApp PRIVATE
    app.cpp
    HandlerFactory.cpp
    HTTPServer.cpp
    MySQLConnection.cpp
    ServerApp.cpp
    UserAuthentication.cpp
)
target_include_directories(tradingApp PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(tradingApp mysqlcppconn CONAN_PKG::poco)

add_subdirectory(handler)