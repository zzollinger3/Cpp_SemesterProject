#include "native_analyzer.h"

void NativeAnalyzer::analyze(const std::vector<FileInfo>& files) {

    // Reset statistics for the current package
    native_count = 0;
    native_bytes = 0;

    // Examine each file
    for (const FileInfo& file : files) {

        if (file.type == FileType::NativeLibrary) {
            native_count++;
            native_bytes += file.size;
        }
    }
}

std::uint64_t NativeAnalyzer::get_native_count() const {
    return native_count;
}

std::uint64_t NativeAnalyzer::get_native_bytes() const {
    return native_bytes;
}