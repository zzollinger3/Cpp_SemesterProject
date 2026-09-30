#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "package_scanner.h"

namespace py = pybind11;

PYBIND11_MODULE(package_scanner, m) {
    py::enum_<FileType>(m, "FileType")
        .value("Python", FileType::Python)
        .value("NativeLibrary", FileType::NativeLibrary)
        .value("Metadata", FileType::Metadata)
        .value("DataOther", FileType::DataOther);
    py::class_<FileInfo>(m, "FileInfo")
        .def_readonly("path", &FileInfo::path)
        .def_readonly("type", &FileInfo::type)
        .def_readonly("size", &FileInfo::size);
    py::class_<ScanResult>(m, "ScanResult")
        .def_readonly("files", &ScanResult::files)
        .def_readonly("total_bytes", &ScanResult::total_bytes);
        m.def("scan_directory", &package_scanner);
}