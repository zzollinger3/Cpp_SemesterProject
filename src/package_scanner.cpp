#include "package_scanner.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Classifying Files Goes Here
FileType classify(const fs::path& path) {
    // Check if the file is inside a .dist-info directory
    for (const auto& part : path) {
        if (part.extension() == ".dist-info") {
            return FileType::Metadata;
        }
    }

    std::string extension = path.extension().string();

    if (extension == ".py") {
        return FileType::Python;
    }

    if (extension == ".so" ||
        extension == ".pyd" ||
        extension == ".dll" ||
        extension == ".dylib") {
        return FileType::NativeLibrary;
    }

    return FileType::DataOther;
}

ScanResult package_scanner(const std::string& dir_path) {
    ScanResult result;

    for (const auto& dir_entry : fs::recursive_directory_iterator(dir_path)) {
        if (!dir_entry.is_regular_file()) continue;

        FileInfo info;
        info.path = fs::relative(dir_entry.path(), dir_path).string();
        info.size = fs::file_size(dir_entry.path());
        info.type = classify(dir_entry.path());

        result.total_bytes += info.size;
        result.files.push_back(std::move(info));
    }

    return result;
}