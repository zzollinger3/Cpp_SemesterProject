cmake_minimum_required(VERSION 3.20)
project(package_scanner LANGUAGES CXX)

add_subdirectory(extern/pybind11)

pybind11_add_module(package_scanner
	src/bindings.cpp
	src/package_scanner.cpp
)
target_compile_features(package_scanner PRIVATE cxx_std_20)
