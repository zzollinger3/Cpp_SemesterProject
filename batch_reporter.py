def create_category_counts():
    return {
        "Python": 0,
        "NativeLibrary": 0,
        "Metadata": 0,
        "DataOther": 0
    }


def update_category_counts(result, counts, package_scanner):
    for file in result.files:
        if file.type == package_scanner.FileType.Python:
            counts["Python"] += 1
        elif file.type == package_scanner.FileType.NativeLibrary:
            counts["NativeLibrary"] += 1
        elif file.type == package_scanner.FileType.Metadata:
            counts["Metadata"] += 1
        elif file.type == package_scanner.FileType.DataOther:
            counts["DataOther"] += 1


def print_inventory_sample(project_name, result):
    print(f"\nInventory sample for {project_name}:")
    print("-" * 70)
    print(f"{'Path':<40} {'Category':<18} {'Size':>10}")
    print("-" * 70)

    for file in result.files[:10]:
        print(f"{file.path:<40}{file.type.name:<18}{file.size:>10}")


def process_result(project_name, result, counts, package_scanner, show_sample=False):
    update_category_counts(result, counts, package_scanner)

    if show_sample:
        print_inventory_sample(project_name, result)


def print_summary(counts):
    print("\n=== Batch Summary ===")
    print(f"Python files:         {counts['Python']}")
    print(f"Native libraries:     {counts['NativeLibrary']}")
    print(f"Metadata files:       {counts['Metadata']}")
    print(f"Data/Other files:     {counts['DataOther']}")