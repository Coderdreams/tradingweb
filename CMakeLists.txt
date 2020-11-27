cmake_minimum_required(VERSION 3.14.1)

project(
    TradingApp
    LANGUAGES CXX)

add_definitions("-std=c++2a")

set_property(GLOBAL PROPERTY CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(conanbuildinfo.cmake)

conan_basic_setup(TARGETS)

add_executable(tradingApp "")

add_executable(testsuite "")

add_subdirectory(trading/src)

add_subdirectory(trading/testsuite/src)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(tradingApp PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
