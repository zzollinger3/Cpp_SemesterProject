#pragma once
#include <string>
#include <vector>
#include "package_scanner.h"   // where PackageMetadata + FileInfo live

PackageMetadata parse_metadata(
    const std::string& root,
    const std::vector<FileInfo>& files
);