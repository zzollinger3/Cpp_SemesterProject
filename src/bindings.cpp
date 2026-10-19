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
    py::class_<EntryPoint>(m, "EntryPoint")
        .def(py::init<>())
        .def_readwrite("group", &EntryPoint::group)
        .def_readwrite("name", &EntryPoint::name)
        .def_readwrite("target", &EntryPoint::target);
    py::class_<PackageMetadata>(m, "PackageMetadata")
        .def(py::init<>())
        .def_readwrite("name", &PackageMetadata::name)
        .def_readwrite("version", &PackageMetadata::version)
        .def_readwrite("requires_python", &PackageMetadata::requires_python)
        .def_readwrite("dependencies", &PackageMetadata::dependencies)
        .def_readwrite("entry_points", &PackageMetadata::entry_points);
    py::class_<ScanResult>(m, "ScanResult")
        .def_readonly("files", &ScanResult::files)
        .def_readonly("total_bytes", &ScanResult::total_bytes)
        .def_readonly("metadata", &ScanResult::metadata)
        .def_readonly("native_count", &ScanResult::native_count)
        .def_readonly("native_bytes", &ScanResult::native_bytes);
        m.def("scan_directory", &package_scanner);
}