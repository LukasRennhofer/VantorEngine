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
 * File: vtrgConstants.py
 * Last Change: Updated the projects build system
"""

from datetime import datetime
from pathlib import Path

# Versioning
MAIN_AUTHOR = "Lukas Rennhofer"
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
 *  Copyright (c) {COPYRIGHT_YEAR} {MAIN_AUTHOR}
 *  Licensed under the {LICENSE}.
 *  See LICENSE file for more details.
 *
 *  Author: {MAIN_AUTHOR}
 *  Date: {datetime.today().strftime("%Y-%m-%d")}
 *
 *  File: {{FILENAME}}
 *  Last Change: Automatically updated
 */
"""

# Paths
MAIN_ENGINE_DIR  = Path("Vantor")
SRC_DIR_INTERNAL = MAIN_ENGINE_DIR / Path("Source")
BUILD_DIR_INTERNAL = Path("Build")

# Resources
SHADERS_DIR = MAIN_ENGINE_DIR / Path("Shaders")
RESOURCE_DIR = MAIN_ENGINE_DIR / Path("Resources")

# Projects
EXAMPLES_INTERNAL = Path("Samples")
SANBOX_DIR_INTERNAL = Path("Sandbox")
INCLUDE_DIR = BUILD_DIR_INTERNAL / "include"
EXTERNAL_SRC_DIR = MAIN_ENGINE_DIR / "External"

# TOOLS
MAIN_PY_ENTRY = "vtrg.py"

# Building
VALID_BUILDING_PLATFORMS = ["Windows", "Linux", "Switch"]
VALID_PLATFORM_PREFIXES = {"Windows" : ["Win32", "Win64"], "Linux" : ["Linux"], "Switch" : ["Homebrew", "NSX"]}
VALID_SAMPLES = ["TestFramework", "Template"]
