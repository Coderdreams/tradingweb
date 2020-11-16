target_sources(testsuite PRIVATE
    Main.cpp
    HTTPServerTest.cpp
    ../../src/MySQLConnection.cpp
)
target_include_directories(testsuite PRIVATE ${CMAKE_CURRENT_LIST_DIR} ../../src)
target_link_libraries(testsuite pthread mysqlcppconn CONAN_PKG::poco CONAN_PKG::cppunit)
