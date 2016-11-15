mkdir temp
cd temp

cmake -G "Visual Studio 14 2015 Win64" ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake -DCMAKE_INSTALL_PREFIX=../install -P cmake_install.cmake

