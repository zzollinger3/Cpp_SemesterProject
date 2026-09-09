# CMake Lists text file

cmake_minimum_required(VERSION 3.12)
project(CppProject)
add_executable(CppProject src/main.cpp)
set_property(TARGET CppProject PROPERTY CXX_STANDARD 20)
