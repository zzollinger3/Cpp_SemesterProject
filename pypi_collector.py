from urllib.request import Request, urlopen
import json
import os
import sys
import urllib
import tempfile,zipfile


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


def choose_wheel(wheels):
    # Prefer py3-none-any wheels
    for w in wheels:
        if "py3-none-any" in w["filename"]:
            return w
    # Otherwise pick the first in sorted order
    return sorted(wheels, key=lambda x: x["filename"])[0]


def download_wheel(url, filename):
    path = os.path.join("wheelhouse", filename)
    urllib.request.urlretrieve(url, path)
    return path

def main():
	if len(sys.argv) < 2:
		print("Usage: python pypi_collector.py candidates.txt")
		return

	candidates_file = sys.argv[1]

	# Create wheelhouse folder if missing
	os.makedirs("wheelhouse", exist_ok=True)

	with open(candidates_file) as f:
		candidates = [line.strip() for line in f if line.strip()]

	count = 0

	for c in candidates:
		if count >= 100:
			break
	
		print(f"Fetching {c}...")

		try:
			version, wheels = fetch_project(c)

			if not wheels:
				print("Skip, no wheels found")
				continue
			wheel = choose_wheel(wheels)
			wheel_path = download_wheel(wheel["url"], wheel["filename"])
	
			print(f"Downloaded {wheel_path}")
			count += 1
	
		except Exception as e:
			print(f"Error: {e}")
			continue


if __name__ == "__main__":
	main()