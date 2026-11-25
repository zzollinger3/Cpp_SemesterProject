#include "package_scanner.h"
#include "metadata_parser.h"
#include "native_analyzer.h"
#include "batch_analyzer.h"
#include <iostream>
#include <filesystem>
#include "entry_points_parser.h"

namespace fs = std::filesystem;

// Classifies each file based on its path and extension.
FileType classify(const fs::path& path) {

    std::string filename = path.string();

    if (filename.find(".dist-info") != std::string::npos) {
        return FileType::Metadata;
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


// Scans a directory and returns information about its files.
ScanResult scan_directory(const std::string& dir_path) {

    ScanResult result;

    // Collect information about each file.
    for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {

        if (!entry.is_regular_file()) {
            continue;
        }

        FileInfo file;

        file.path = entry.path().string();
        file.type = classify(entry.path());
        file.size = entry.file_size();

        result.total_bytes += file.size;

        result.files.push_back(file);
    }

    // Analyze native libraries using NativeAnalyzer.
    NativeAnalyzer native_analyzer;
    native_analyzer.analyze(result.files);

    result.native_count = native_analyzer.get_native_count();
    result.native_bytes = native_analyzer.get_native_bytes();

    // Extract package metadata.
    result.metadata = parse_metadata(dir_path, result.files);

    // Extract entry points.
    result.metadata.entry_points =
        parse_entry_points(dir_path, result.files);

    return result;
}
