#include "package_scanner.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Classifying Files Goes Here

ScanResult package_scanner(const std::string& dir_path) {
    ScanResult result;

    for (const auto& dir_entry : fs::recursive_directory_iterator(dir_path)) {
        if (!dir_entry.is_regular_file()) continue;

        FileInfo info;
        info.path = fs::relative(entry.path(), dir_path).string();
        info.size = fs::file_size(entry.path());
        info.type = classify(entry.path());

        result.total_bytes += info.size;
        result.files.push_back(std::move(info));
    }

    return result;
}