cmake_minimum_required(VERSION 3.20)
project(package_scanner LANGUAGES CXX)

add_subdirectory(extern/pybind11)

pybind11_add_module(package_scanner
	src/bindings.cpp
	src/package_scanner.cpp
	src/native_analyzer.cpp
	src/batch_analyzer.cpp
	src/metadata_parser.cpp
	src/entry_points_parser.cpp
)
target_compile_features(package_scanner PRIVATE cxx_std_20)

set_target_properties(package_scanner PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python"
)