#pragma once

#include <cstdint>
#include <vector>
#include "package_scanner.h"

class NativeAnalyzer {
private:
    std::uint64_t native_count = 0;
    std::uint64_t native_bytes = 0;

public:
    NativeAnalyzer() = default;
    ~NativeAnalyzer() = default;

    void analyze(const std::vector<FileInfo>& files);

    std::uint64_t get_native_count() const;
    std::uint64_t get_native_bytes() const;
};