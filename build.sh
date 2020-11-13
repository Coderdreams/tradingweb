conan install --build=missing -s compiler=gcc -s compiler.version=9 -s compiler.libcxx=libstdc++11 .
premake5 gmake
make
