# cmake_minimum_required(VERSION <specify CMake version here>)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project(PolyTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(PolyTest main.cpp Shapes.cpp Image.cpp)