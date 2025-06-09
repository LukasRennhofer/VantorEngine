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


# ====================== Console Application ============================
class vtrgCLIClient:
    def __init__(self):
        self.build_system = VantorInternalBuildSystem()

    def parseArgs(self):
        parser = argparse.ArgumentParser(description="VTRG (Vantor Trigger Intern CLI)")
        subparsers = parser.add_subparsers(dest='command', required=True)

        # === build ===
        build_parser = subparsers.add_parser('build', help='Build internal modules, samples, or the sandbox')
        build_parser.add_argument('--sample', type=str, help='Build a sample project from the Samples directory')
        build_parser.add_argument('--platform', type=str, choices=["Windows", "Linux", "Switch"], help='Target platform')
        build_parser.add_argument('--clean', type=str, help="Directory to clean before building")
        build_parser.add_argument('--debug', action='store_true', help="Enable debug build")
        build_parser.add_argument('--run', action='store_true', help="Run directly after building")
        build_parser.add_argument('--check-dependencies', action='store_true', help="Check if dependencies like CMake are installed")
        build_parser.set_defaults(func=self.handle_build)

        # === format ===
        format_parser = subparsers.add_parser('format', help='Format the codebase or modules')
        format_parser.set_defaults(func=self.handle_format)

        # === config ===
        config_parser = subparsers.add_parser('config', help='TODO: Configuration management')
        config_parser.set_defaults(func=self.handle_config)

        return parser.parse_args()

    def handle_build(self, args):
        if args.clean:
            print("🧹 Cleaning build directory...")
            self.build_system.cleanBuildDir(args.clean)

        if args.check_dependencies:
            if self.build_system.check_cmake_installed():
                print("✅ CMake is installed!")
            else:
                print("❌ CMake is not installed!")

        print("🔧 Starting build...")

        if args.sample:
            executable_path = self.build_system.buildProject(
                target=args.platform,
                exampleName=args.sample,
                debugging=args.debug
            )
            
            if args.run:
                if os.path.exists(executable_path):
                    print(f"🛠️  Running executable: {executable_path}")
                    subprocess.run(["./"+executable_path])
                else:
                    print(f"❌  Executable not found at {executable_path}")
        else:
            self.build_system.buildIntern(
                target=args.platform,
                debugging=args.debug
            )

    def handle_format(self, args):
        print("🎨 Formatting Vantor source code...")
        format_files_in_folder(SRC_DIR_INTERNAL)

    def handle_config(self, args):
        print("🛠️ Configuration management is not yet implemented.")

    def execute(self):
        args = self.parseArgs()
        args.func(args)