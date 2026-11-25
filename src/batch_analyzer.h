#pragma once

#include <vector>
#include "package_scanner.h"

class BatchAnalyzer {
public:
    BatchAnalyzer() = default;
    ~BatchAnalyzer() = default;

    BatchSummary summarize_batch(const std::vector<ScanResult>& results);
};