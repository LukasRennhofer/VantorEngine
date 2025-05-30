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
 * File: extern.py
 * Last Change: Updated the projects build system
"""

"""
============================================================================ INFO ===============================================================================================
This module is used to build a Vantor project and its baselib. It uses the cmake build system and the make compile manager to compile the source code and create the final libaries and executables.
It is designed to be run from the command line (TODO: Can support GUI with VTRGGUI) and takes care of setting up the environment, compiling the source code, and linking the final 
executables.
"""

import argparse

from Tools.vtrg.Misc.vtrgConstants import *
from Tools.vtrg.Formatting.vtrgFormat import *
from Tools.vtrg.Console.vtrgCLIUtils import vtrgCLIClientUtils
from Tools.vtrg.Building.vtrgBSystem import VantorInternalBuildSystem

# ====================== Console Application (In Work) ============================
class vtrgCLIClient:
    def __init__(self):
        self.build_system = VantorInternalBuildSystem()
    
    def parseArgs(self):
        parser = argparse.ArgumentParser(description="VTRG (Vantor Trigger Intern CLI)")
        
        parser.add_argument(
            "--platform", 
            type=str, 
            choices=["Windows", "Linux", "Switch"], 
            help="Specify the target platform for the build (Windows, Linux, Switch)"
        )

        parser.add_argument(
            "--format", 
            action="store_true", 
            help="Formats the whole Vantor CodeBase using a custom Header and clang-format"
        )

        parser.add_argument(
            "--build-lib", 
            action="store_true", 
            help="Build the Vantor libaries"
        )

        parser.add_argument(
            "--clean", 
            type=str,
            help="Directory to clean (e.g., 'build/windows')"
        )

        parser.add_argument(
            "--check-cmake", 
            action="store_true", 
            help="Check if cmake is installed"
        )

        parser.add_argument(
            "--build-examples", 
            type=str, 
            help="Build example projects (e.g., \"TestFramework\")"
        )

        parser.add_argument(
            "--build-sandbox", 
            action="store_true", 
            help="Build internal Sandbox Project"
        )

        parser.add_argument(
            "--debug-build", 
            action="store_true", 
            help="See all the debugging infos while building"
        )
        
        return parser.parse_args()

    def executeBuild(self, args):
        debugging = False
        if args.clean:
            print("Cleaning build...")
            self.build_system.cleanBuildDir(args.clean)
        
        if args.debug_build:
            debugging = True
        
        if args.check_cmake:
            if self.build_system.check_cmake_installed():
                print("✅ CMake is installed!")
            else:
                print("❌ CMake is not installed!")

        if args.build_lib:
            self.build_system.buildLib(target=args.platform, debugging=debugging)

        if args.build_examples:
            self.build_system.buildExample(target=args.platform, exampleName=args.build_examples, debugging=debugging)

        if args.build_sandbox:
            self.build_system.buildSanbox(target=args.platform, debugging=debugging)

        if args.format:
            vantorFormat.format_files_in_folder(SRC_DIR_INTERNAL)