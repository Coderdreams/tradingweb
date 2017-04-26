#!lua

require 'conanpremake'

solution "TradingApp"
   configurations { "Debug", "Release" }
   includedirs { "trading/src", conan_includedirs }
   libdirs { conan_libdirs }
   links { "mysqlcppconn", conan_libs }
   configuration { "linux", "gmake" }
      buildoptions { "-Wall", "-std=c++14"}
   -- A project defines one build target
   project "tradingApp"
      kind "ConsoleApp"
      language "C++"
      files { "trading/src/**.cpp" }

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
      links { "cppunit", conan_libs }
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         objdir ("objs")
