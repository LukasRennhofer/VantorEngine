import os
import re
from constants import *

root_dir = ""
dev_env = ""


def generate_header(version, author, file_name, rootf):
    if rootf == SRC_DIR:

        return  f"""\
/*
* File Codec: {file_name}@CHIFEngine
* Author: {author}
* Version Codec: {version}@CHIFEngine
*/
"""

    else:
        if rootf in EXTERN_RESSOURCES:
            author_n = EXTERN_RESSOURCES[rootf]['author']
            description_n = EXTERN_RESSOURCES[rootf]['description']
            website_n = EXTERN_RESSOURCES[rootf]['website']
            return  f"""\
/*
* Module Codec: {rootf}::{file_name}@CHIFEngine
* Author: {author_n}
* Description: {description_n}
* Website: {website_n}
*/
"""

        else:
            return  f"""\
/*
* File Codec: {file_name}@CHIFEngine
* Author: {author}
* - UNKNOWN REGISTRED FILE - 
* Version Codec: {version}@CHIFEngine
*/
"""

def remove_comments_and_add_header(file_path):
    global root_dir

    with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
        content = file.read()

    content = re.sub(r'//.*', '', content)
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
    
    content = generate_header(GLOBAL_VERSION, MAINTAIN_AUTHOR, os.path.basename(file_path), root_dir) + "\n" + content.strip()

    with open(file_path, 'w', encoding='utf-8', errors='ignore') as file:
            file.write(content)


def process_directory(directory):
    global root_dir

    for root, _, files in os.walk(directory):
        for file in files:
            if os.path.basename(root) in EXTERN_RESSOURCES or os.path.basename(root) == SRC_DIR:
                root_dir = os.path.basename(root)
            file_path = os.path.join(root, file)
            if file.endswith(('.c', '.cpp', '.h','.py')):
                print(f"Processing {file_path}")
                remove_comments_and_add_header(file_path)


if __name__ == "__main__":
    directory_to_process = os.getcwd()
    process_directory(directory_to_process)
    print("Processing complete.")


