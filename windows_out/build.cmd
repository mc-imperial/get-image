mkdir -p temp
cd temp

echo TODO:

echo CMAKE_INCLUDE_PATH
echo CMAKE_LIBRARY_PATH

cmake -G "Visual Studio 14 2015 Win64" ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake -DCMAKE_INSTALL_PREFIX=../install -P cmake_install.cmake

