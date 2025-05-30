#!/usr/bin/env python3

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
 * File: vtrg.py
 * Last Change: Updated the projects build system
"""

"""
This file contains the Vantor Dev Environment Console Application VTRG (Vantor Trigger) for controlling the internal
code structure and build system
"""

from Tools.vtrg.extern import *

if __name__ == "__main__":
     client = vtrgCLIClient() 
     args = client.parseArgs()
     client.executeBuild(args)