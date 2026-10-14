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

struct EntryPoint {
    std::string group;
    std::string name;
    std::string target;
};

struct PackageMetadata {
    std::string name;
    std::string version;
    std::string requires_python; // Empty when absent
    std::vector<std::string> dependencies; // Every Requires-Dist value
    std::vector<EntryPoint> entry_points;
};

struct ScanResult {
    std::vector<FileInfo> files;
    std::uint64_t total_bytes{};
    PackageMetadata metadata;
    std::uint64_t native_count{};
    std::uint64_t native_bytes{};
};

ScanResult package_scanner(const std::string& dir_path);

struct BatchSummary {
    std::size_t packages{};
    std::size_t declared_dependencies{};
    std::size_t packages_with_entry_points{};
    std::size_t packages_with_native_files{};
    std::uint64_t native_files{};
    std::uint64_t native_bytes{};
};