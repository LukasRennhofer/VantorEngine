import os

def find_and_replace_in_files(root_dir, target_string, replacement_string):
    total_replacements = 0
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            filepath = os.path.join(dirpath, filename)
            try:
                with open(filepath, 'r', encoding='utf-8') as file:
                    content = file.read()
                
                count = content.count(target_string)
                if count > 0:
                    print(f"Found in: {filepath} | Occurrences: {count}")
                    # new_content = content.replace(target_string, replacement_string)
                    # with open(filepath, 'w', encoding='utf-8') as file:
                    #     file.write(new_content)
                    # print(f"Replaced in: {filepath}")
                    total_replacements += count
            except (UnicodeDecodeError, PermissionError, IsADirectoryError):
                continue
    print(f"\nTotal replacements across all files: {total_replacements}")

if __name__ == "__main__":
    directory_to_search = input("Enter directory path to search: ").strip()
    search_string = input("Enter string to search for: ").strip()
    replace_string = input("Enter replacement string: ").strip()

    find_and_replace_in_files(directory_to_search, search_string, replace_string)