#include "batch_analyzer.h"

BatchSummary BatchAnalyzer::summarize_batch(
    const std::vector<ScanResult>& results
) {
    BatchSummary summary;

    summary.packages = results.size();

    for (const ScanResult& result : results) {

        // Count dependencies
        summary.declared_dependencies +=
            result.metadata.dependencies.size();

        // Count packages with entry points
        if (!result.metadata.entry_points.empty()) {
            ++summary.packages_with_entry_points;
        }

        // Count packages with native files
        if (result.native_count > 0) {
            ++summary.packages_with_native_files;
        }

        // Aggregate native statistics
        summary.native_files += result.native_count;
        summary.native_bytes += result.native_bytes;

        // Count individual file categories
        for (const FileInfo& file : result.files) {

            switch (file.type) {

            case FileType::Python:
                ++summary.python_files;
                break;

            case FileType::NativeLibrary:
                // Already counted in native_files
                break;

            case FileType::Metadata:
                ++summary.metadata_files;
                break;

            case FileType::DataOther:
                ++summary.other_files;
                break;
            }
        }
    }

    return summary;
}