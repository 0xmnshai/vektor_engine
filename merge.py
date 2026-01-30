import os

ROOT_DIR = "./"
OUTPUT_FILE = "all_files"

# Normalize paths for OS-independent matching
EXCLUDED_DIRS = {
    os.path.normpath("build"),
    os.path.normpath(".vscode"),
    os.path.normpath("vektor/vendor"),
    os.path.normpath(".git"),
    os.path.normpath("merge.py"),
    os.path.normpath("logs"),
    os.path.normpath("imgui.ini")
}

def is_text_file(path):
    try:
        with open(path, "r", encoding="utf-8") as f:
            f.read(1024)
        return True
    except:
        return False

def should_skip_dir(dirpath):
    dirpath = os.path.normpath(dirpath)
    for excluded in EXCLUDED_DIRS:
        if dirpath.endswith(excluded):
            return True
    return False

def collect_files(root):
    all_files = []

    for dirpath, dirnames, filenames in os.walk(root):
        # Remove excluded folders from traversal
        dirnames[:] = [
            d for d in dirnames
            if not should_skip_dir(os.path.join(dirpath, d))
        ]

        for file in filenames:
            full_path = os.path.join(dirpath, file)
            if is_text_file(full_path):
                all_files.append(full_path)

    return all_files

def merge_files(file_list, output_file):
    with open(output_file, "w", encoding="utf-8") as out:
        for file in file_list:
            try:
                with open(file, "r", encoding="utf-8") as f:
                    content = f.read()
            except:
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
