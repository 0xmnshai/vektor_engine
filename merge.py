import os

ROOT_DIR = "./"
OUTPUT_FILE = "all_files"

# Normalize paths for OS-independent matching
EXCLUDED_PATHS = {
    os.path.normpath("build"),
    os.path.normpath("build.sh"),
    os.path.normpath(".vscode"),
    os.path.normpath("vektor/vendor"),
    os.path.normpath(".git"),
    os.path.normpath("merge.py"),
    os.path.normpath("logs"),
    os.path.normpath("imgui.ini"),
    os.path.normpath(OUTPUT_FILE),
    os.path.normpath("Readme.md"),
    os.path.normpath("CMakelists.txt"),
    os.path.normpath("resources.md"), 
}

def is_text_file(path):
    try:
        with open(path, "r", encoding="utf-8") as f:
            f.read(1024)
        return True
    except (IOError, UnicodeDecodeError):
        return False

def should_skip_path(path):
    normalized_path = os.path.normpath(path)
    for excluded in EXCLUDED_PATHS:
        if normalized_path == excluded or normalized_path.startswith(excluded + os.sep):
            return True
    return False

def collect_files(root):
    all_files = []

    for dirpath, dirnames, filenames in os.walk(root):
        # Remove excluded folders from traversal
        dirnames[:] = [
            d for d in dirnames
            if not should_skip_path(os.path.join(dirpath, d))
        ]

        for file in filenames:
            full_path = os.path.join(dirpath, file)
            if not should_skip_path(full_path) and is_text_file(full_path):
                all_files.append(full_path)

    return all_files

def merge_files(file_list, output_file):
    with open(output_file, "w", encoding="utf-8") as out:
        for file in file_list:
            try:
                with open(file, "r", encoding="utf-8") as f:
                    content = f.read()
            except IOError:
                continue

            out.write(f"file name : {file}\n")
            out.write(content)
            out.write("\n\n----\n\n")

def main():
    files = collect_files(ROOT_DIR)
    merge_files(files, OUTPUT_FILE)
    print(f"Merged {len(files)} files into {OUTPUT_FILE}")

if __name__ == "__main__":
    main()
