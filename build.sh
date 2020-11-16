conan install --build=missing -s compiler=gcc -s compiler.version=9 -s compiler.libcxx=libstdc++11 .
# Changing from premake to cmake because IDEs work better with it.
#premake5 gmake
mkdir -p build
pushd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make
popd
source activate_run.sh
