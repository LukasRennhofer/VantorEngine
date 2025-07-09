#!/usr/bin/env python3

"""
 *    				~ Vantor ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-07-09
 *
 * File: vtrg.py
 * Description: Main entry point for the Vantor Trigger (VTRG) development tool
"""

"""
VTRG (Vantor Trigger) - Development Tool for Vantor Engine

This tool provides a comprehensive command-line interface for:
- Building Vantor engine and sample projects
- Code formatting and style enforcement
- Configuration management
- Build artifact cleanup
- Development workflow automation

The tool supports multiple platforms (Windows, Linux, Nintendo Switch) and 
provides both simple commands for quick tasks and advanced options for 
complex development scenarios.
"""

import sys
from Tools.vtrg.main import main

if __name__ == "__main__":
    sys.exit(main())