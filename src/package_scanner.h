#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class FileType { Python, NativeLibrary, Metadata, DataOther };

struct FileInfo {
    std::string path;
    FileType type{};
    std::uint64_t size{};
};

struct ScanResult {
    std::vector<FileInfo> files;
    std::uint64_t total_bytes{};
};

ScanResult package_scanner(const std::string& dir_path);