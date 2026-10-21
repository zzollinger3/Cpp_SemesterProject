
#include "entry_points_parser.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <cctype>

namespace fs = std::filesystem;

// Remove whitespace from the beginning and end of a string
static std::string trim(const std::string& str) {
    size_t start = 0;
    while (start < str.size() &&
           std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }

    size_t end = str.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }

    return str.substr(start, end - start);
}

std::vector<EntryPoint> parse_entry_points(
    const std::string& dir_path,
    const std::vector<FileInfo>& files
) {
    std::vector<EntryPoint> entry_points;

    // Find entry_points.txt using the existing file inventory
    for (const FileInfo& file : files) {
        fs::path relative_path(file.path);

        if (relative_path.filename() != "entry_points.txt" ||
            relative_path.parent_path().extension() != ".dist-info") {
            continue;
        }

        // Open the entry points file
        std::ifstream input(fs::path(dir_path) / relative_path);

        if (!input.is_open()) {
            continue;
        }

        std::string line;
        std::string current_group;

        // Read the file line by line
        while (std::getline(input, line)) {
            line = trim(line);

            // Skip empty lines and comments
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }

            // Detect group headings such as [console_scripts]
            if (line.front() == '[' && line.back() == ']') {
                current_group = trim(line.substr(1, line.size() - 2));
                continue;
            }

            // Find the first equals sign
            size_t equals_pos = line.find('=');

            if (equals_pos == std::string::npos ||
                current_group.empty()) {
                continue;
            }

            // Extract the entry point name and target
            std::string name = trim(line.substr(0, equals_pos));
            std::string target = trim(line.substr(equals_pos + 1));

            if (name.empty() || target.empty()) {
                continue;
            }

            // Store the parsed entry point
            entry_points.push_back({
                current_group,
                name,
                target
            });
        }
    }

    return entry_points;
}
