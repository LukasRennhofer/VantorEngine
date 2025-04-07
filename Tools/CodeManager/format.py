"""
*    				~ CHIFEngine ~
*               
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the MIT License. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 03-23-2025
*
* File: format.py
* Last Change: Tooling Updatet
"""

import os
import re
from constants import *
from datetime import datetime

root_dir = ""
dev_env = ""

LAST_CHANGE = ""


def generate_header(version, author, file_name, rootf):

    return  f"""
/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 {author}
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: {author}
 * Date: {datetime.today().strftime('%Y-%m-%d')}
 *
 * File: {file_name}
 * Last Change: {LAST_CHANGE}
 */
 
"""

def add_header(file_path):
    global root_dir

    with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
        content = file.read()
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
            if file.endswith(('.c', '.cpp', '.h')):
                print(f"Processing {file_path}")
                add_header(file_path)


if __name__ == "__main__":
    directory_to_process = os.getcwd()
    process_directory(directory_to_process)
    print("Processing complete.")


