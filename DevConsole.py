"""
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-04-07
 *
 * File: DevConsole.py
 * Last Change: Updated the projects build system
"""

"""
This file contains the CHIFEngine Dev Environment Console Application for controlling the internal files
"""

from Tools.chifDev import *

if __name__ == "__main__":
     console = CHIFDevConsole()
     args = console.parseArgs()
     console.executeBuild(args)
