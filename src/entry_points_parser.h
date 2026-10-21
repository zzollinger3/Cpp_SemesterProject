
#pragma once

#include "package_scanner.h"

#include <string>
#include <vector>

std::vector<EntryPoint> parse_entry_points(
    const std::string& dir_path,
    const std::vector<FileInfo>& files
);
