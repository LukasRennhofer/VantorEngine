"""
 *    				~ Vantor ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-04-07
 *
 * File: DevConsole.py
 * Last Change: Updated the projects build system
"""

"""
This file contains the Vantor Dev Environment Console Application for controlling the internal files
"""

from Tools.vantorDev import *

if __name__ == "__main__":
     console = VANTORDevConsole() 
     args = console.parseArgs()
     console.executeBuild(args)