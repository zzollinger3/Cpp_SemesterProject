from urllib.request import Request, urlopen
import json
import os
import sys
import urllib
import tempfile,zipfile
import shutil
<<<<<<< HEAD
import package_scanner
from batch_reporter import (
    create_category_counts,
    process_result,
    print_summary
)

=======
>>>>>>> 3d4d408 (updated code)


# read the project names and return the wheels 
def fetch_project(project):
	url = f"https://pypi.org/pypi/{project}/json"
	req = Request(url, headers={"User-Agent": "CS3460-PackageScanner/1.0"})
	with urlopen(req, timeout=20) as response:
		data = json.load(response)
	wheels = [
		f for f in data["urls"]
		if f["packagetype"] == "bdist_wheel" and not f.get("yanked", False)
	]
	return data["info"]["version"], wheels


# picks one wheel to work with preferring py3-none-any when possible
def choose_wheel(wheels):
    for w in wheels:
        if "py3-none-any" in w["filename"]:
            return w
    return sorted(wheels, key=lambda x: x["filename"])[0]


# downloads a single wheel file and returns its path to be accessed by other functions
def download_wheel(url, filename):
    path = os.path.join("wheelhouse", filename)
    urllib.request.urlretrieve(url, path)
    return path


# use package_scanner to get the data needed from each wheel file we unzip
def scan_package(wheel_path):
	with tempfile.TemporaryDirectory() as tmp:
		with zipfile.ZipFile(wheel_path) as zf:
			zf.extractall(tmp)
		result = package_scanner.scan_directory(tmp)
		return result
		# result owns the metadata needed after tmp is deleted
		


def main():
	if len(sys.argv) < 2:
		print("Usage: python pypi_collector.py packages.txt")
		return

	packages_file = sys.argv[1]

	# create the wheelhouse directory each time
	os.makedirs("wheelhouse", exist_ok=True)

	# separates each project name so it can be easily traversed one at a time
	with open(packages_file) as f:
		packages = [line.strip() for line in f if line.strip()]

	count = 0  # so it can stop at 100 good wheel files
	manifest = [] # to hold the individual wheel data

	# Part 4 reporting data
	counts = create_category_counts()

	for p in packages:
		if count >= 100:  # changed to 10 for testing purposes
			break
	
		print(f"Fetching {p}...")

		try:
			version, wheels = fetch_project(p)

			if not wheels:
				print("Skip, no wheels found")
				continue

			wheel = choose_wheel(wheels)
			wheel_path = download_wheel(wheel["url"], wheel["filename"])
			print(f"Downloaded {wheel_path}")
			
			result = scan_package(wheel_path)

			process_result(
				p,
				result,
				counts,
				package_scanner,
				show_sample=(count == 0)
			)

			# store all the relevant data from the wheel to add to the JSON object later
			entry = {
				"project": p,
				"version": version,
				"wheel": wheel["filename"],
				"url": wheel["url"],
				"total_bytes": result.total_bytes,
				"files": [
					{
						"path": fi.path,
						"type": fi.type.name,
						"size": fi.size
					}
					for fi in result.files
				]
			}

			manifest.append(entry)

			print(f"Scanned {len(result.files)} files")
			count += 1
	
		except Exception as e:
			print(f"Error: {e}")
			continue
	print_summary(counts)

	# write all the collected data into a JSON
	with open("manifest.json", "w") as f:
		json.dump(manifest, f, indent=2)

	# delete the wheelhouse directory after use to keep workspace clean
	shutil.rmtree("wheelhouse", ignore_errors=True)




if __name__ == "__main__":
	main()