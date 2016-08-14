conan install --build=missing -s compiler=gcc -s compiler.version=5.4 -s compiler.libcxx=libstdc++11
premake4 gmake
make
