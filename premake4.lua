#!lua

include("conanbuildinfo.premake.lua")
--require 'conanpremake'
--include("premake-cmake-master/cmake_cpp.lua")

solution "TradingApp"
   configurations { "Debug", "Release" }
   includedirs { "trading/src", conan_includedirs }
   libdirs { conan_libdirs }
   links { "mysqlcppconn", conan_libs }
   configuration { "linux", "gmake" }
      buildoptions { "-Wall", "-Wextra", "-Wpedantic", "-std=c++2a" }
   -- A project defines one build target
   project "tradingApp"
      kind "ConsoleApp"
      language "C++"
      files { "trading/src/**.?pp" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         objdir ("objs")

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
         objdir ("objs")

   project "testsuite"
      kind "ConsoleApp"
      language "C++"
      files { "trading/testsuite/src/**.cpp", "trading/src/MySQLConnection.cpp" }
      links { "cppunit", "pthread", conan_libs }
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         objdir ("objs")
