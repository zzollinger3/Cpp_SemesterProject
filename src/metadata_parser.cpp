#include "metadata_parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>

PackageMetadata parse_metadata(const std::string& root, const std::vector<FileInfo>& files) {
    PackageMetadata meta;

    // find METADATA file
    std::string metadata_path;
    for (const auto& f : files) {
        std::string fixed = f.path;
        std::replace(fixed.begin(), fixed.end(), '\\', '/'); // make the slashes normal for all systems

        if (fixed.find("METADATA") != std::string::npos &&
            fixed.find("dist-info") != std::string::npos) {
            metadata_path = root + "/" + fixed;
            break;
        }
    }

    if (metadata_path.empty()) {
        return meta; 
    }

    // open file
    std::ifstream in(metadata_path);
    if (!in) {
        return meta;
    }

    // read header section until a blank line
    std::string line;
    std::string current_field;
    std::string current_value;

    auto flush_field = [&](const std::string& field, const std::string& value) {
        if (field == "Name") {
            meta.name = value;
        }
        else if (field == "Version") {
            meta.version = value;
        } 
        else if (field == "Requires-Python") {
            meta.requires_python = value;
        }
        else if (field == "Requires-Dist") {
            meta.dependencies.push_back(value); // uses push back because there can be multiple dependencies
        }
    };

    while (std::getline(in, line)) {
        // quit at the end of the header section
        if (line.empty()) break; 

        // continue parsing if it's a space or new line
        if (!line.empty() && (line[0] == ' ' || line[0] == '\t')) {
            current_value += line;
            continue;
        }

        // new field
        if (!current_field.empty()) {
            flush_field(current_field, current_value);
        }

        auto pos = line.find(':');
        if (pos != std::string::npos) {
            current_field = line.substr(0, pos);
            current_value = line.substr(pos + 1);
            // trim any whitespace
            current_value.erase(0, current_value.find_first_not_of(" \t"));
        }
    }

    // store the last field
    if (!current_field.empty()) {
        flush_field(current_field, current_value);
    }

    return meta;
}
