"""
 *    				~ Vantor ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 
 *
 * File: vantorFormat.py
 * Last Change: Updated the projects build system
"""

import os
import sys
import re
from datetime import datetime
import subprocess

# === CONFIGURATION ===
AUTHOR = "Lukas Rennhofer"
COPYRIGHT_YEAR = "2025"
LICENSE = "GNU General Public License, Version 3"
ENGINE_NAME = "Vantor"
HEADER_STYLE = f"""/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ {ENGINE_NAME} ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the {ENGINE_NAME} Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) {COPYRIGHT_YEAR} {AUTHOR}
 *  Licensed under the {LICENSE}.
 *  See LICENSE file for more details.
 *
 *  Author: {AUTHOR}
 *  Date: {datetime.today().strftime("%Y-%m-%d")}
 *
 *  File: {{FILENAME}}
 *  Last Change: Automatically updated
 */
"""

INCLUDE_PATTERN = re.compile(r'^\s*#\s*include\s+[<"].+[>"].*$', re.MULTILINE)

def format_cpp_file(file_path):
    try:
        result = subprocess.run(
            ['clang-format', '-i', file_path],
            capture_output=True,
            text=True
        )

        if result.returncode != 0:
            print(f"Error formatting file {file_path}: {result.stderr}")
        else:
            print(f"Formatted {file_path}")

    except Exception as e:
        print(f"Error with file {file_path}: {e}")

def format_files_in_folder(folder_path):
    for root, dirs, files in os.walk(folder_path):
        if 'External' in dirs:
            dirs.remove('External')

        for file_name in files:
            if file_name.endswith('.cpp') or file_name.endswith('.hpp') or file_name.endswith('.h'):
                file_path = os.path.join(root, file_name)
                formatHeaderinfile(file_path)
                formatHeaderinfile(file_path)
                format_cpp_file(file_path)

def sort_includes(lines):
    system_includes = []
    local_includes = []
    other_lines = []

    for line in lines:
        if line.strip().startswith("#include"):
            if "<" in line:
                system_includes.append(line.strip())
            else:
                local_includes.append(line.strip())
        else:
            other_lines.append(line)

    system_includes = sorted(set(system_includes))
    local_includes = sorted(set(local_includes))

    return system_includes + [""] + local_includes + [""]

def insert_header(content, filename):
    header_pattern = re.compile(
        r"^\s*/\*\n(?: \*.*\n)+ \*/\n*", re.MULTILINE
    )

    content = re.sub(header_pattern, "", content)

    header = HEADER_STYLE.replace("{FILENAME}", os.path.basename(filename))
    return header.strip() + "\n\n" + content.lstrip()


def auto_comment_code(content):
    lines = content.splitlines()
    result = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith("class ") and "{" in line:
            result.append("// Class declaration")
        elif re.match(r'.*\)\s*{', stripped) and ("(" in stripped):
            result.append("// Function definition")
        result.append(line)
    return "\n".join(result)

def formatHeaderinfile(path):
    with open(path, "r", encoding="utf-8") as file:
        content = file.read()

    new_content = insert_header(content, path)

    with open(path, "w", encoding="utf-8") as file:
        file.write(new_content)

    print(f"Formatted: {path}")

def format_directory(path):
    for root, _, files in os.walk(path):
        for f in files:
            if f.endswith(".cpp") or f.endswith(".hpp") or f.endswith(".h") or f.endswith(".c"):
                formatHeaderinfile(os.path.join(root, f))

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python vantor_formatter.py <file-or-directory>")
        sys.exit(1)

    target = sys.argv[1]

    if os.path.isfile(target):
        formatHeaderinfile(target)
        format_cpp_file(target)
    elif os.path.isdir(target):
        format_directory(target)
    else:
        print("Invalid file or directory")
