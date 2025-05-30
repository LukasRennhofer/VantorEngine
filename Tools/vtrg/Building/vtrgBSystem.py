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
 * File: vtrgBsystem.py
 * Last Change: Updated the projects build system
"""

import os
import sys
import subprocess
import shutil
import platform
from pathlib import Path
import time

from Tools.vtrg.Misc.vtrgConstants import *
from Tools.vtrg.Console.vtrgCLIUtils import vtrgCLIClientUtils

# ======================= Internal Build System (For the Libaries and Includes, neeeds to be updated @LRDev - 4.3) =================
class VantorInternalBuildSystem:
    """
    VantorInternalBuildSystem handles the build process for Vantor, including library compilation,
    resource copying, and header file collection.
    """

    def __init__(self):
        """
        Initializes the build system by detecting the running platform.
        """
        self.runningSystem = self.check_running_platform()
        self.buildingSystem = self.runningSystem if self.runningSystem else None

    def check_running_platform(self):
        """
        Detects the current operating system.

        Returns:
            str: The name of the platform ("Windows", "Linux", or None if unsupported).
        """
        system_platform = platform.system()
        if system_platform == "Windows":
            return "Windows"
        elif system_platform == "Linux":
            return "Linux"
        else:
            return None

    def check_cmake_installed(self):
        """
        Checks if CMake is installed on the system.

        Returns:
            bool: True if CMake is installed, False otherwise.
        """
        try:
            result = subprocess.run(['cmake', '--version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            return result.returncode == 0
        except FileNotFoundError:
            return False

    def buildLib(self, debugging=False, target=None):
        """
        Builds the Vantor library for the specified platform.

        Args:
            target (str): The target platform for the build (e.g., "Windows", "Linux", "Switch").
        """
        if not target:
            target = self.runningSystem

        if target not in VALID_BUILDING_PLATFORMS:
            print(f"❌ Unsupported platform: {target}")
            return

        build_path = BUILD_DIR_INTERNAL / target
        build_path.mkdir(parents=True, exist_ok=True)

        # CMake Build
        print(f"🔧 Starting CMake build for {target}...")
        result = subprocess.Popen([
            "cmake", "-DPLATFORM=" + target, "-G", "Unix Makefiles", str(SRC_DIR_INTERNAL.resolve())
        ], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, "Building Cmake Configs")

        stdout, stderr = result.communicate()

        if result.returncode != 0:
            print("❌ CMake configuration failed.")
            print(f"⚠️ CMakeError: {stderr.decode()}")
            return

        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

        # Make Build
        result = subprocess.Popen(["make"], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, "Building the library")

        stdout, stderr = result.communicate()

        if result.returncode == 0:
            print("✅ Build successful!")
            self.copy_resources(build_path)
            self.collect_includes()
        else:
            print("❌ Build failed.")
            print(f"⚠️ MakeError: {stderr.decode()}")

        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

    def copy_resources(self, build_path):
        """
        Copies resource directories to the build path.

        Args:
            build_path (Path): The path to the build directory.
        """
        for res_dir in RESOURCE_DIRS:
            src = Path(f"{SRC_DIR_INTERNAL}/" + res_dir)
            dest = build_path / res_dir
            if src.exists():
                shutil.copytree(src, dest, dirs_exist_ok=True)
                print(f"📦 Copied {res_dir} to {dest}")
            else:
                print(f"⚠️ Resource folder not found: {res_dir}")

    def collect_includes(self):
        """
        Collects all header files from the source directory and copies them to the include directory.
        """
        print("📁 Collecting header files into include directory...")
        INCLUDE_DIR.mkdir(parents=True, exist_ok=True)
        for root, _, files in os.walk(SRC_DIR_INTERNAL):
            for file in files:
                if file.endswith(".h") or file.endswith(".hpp"):
                    full_path = Path(root) / file
                    rel_path = full_path.relative_to(SRC_DIR_INTERNAL)
                    dest_path = INCLUDE_DIR / rel_path
                    dest_path.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(full_path, dest_path)
        print(f"✅ Header files copied to {INCLUDE_DIR}")

    def cleanBuildDir(self, build_dir):
        """
        Cleans the specified build directory by running 'make clean'.

        Args:
            build_dir (str): The path to the build directory to clean.
        """
        build_path = os.path.abspath(build_dir)

        if os.path.exists(build_path) and os.path.isdir(build_path):
            try:
                print(f"🧹 Cleaning build in '{build_path}'...")
                result = subprocess.run(['make', 'clean'], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                
                if result.returncode == 0:
                    print("✅ Build cleaned successfully!")
                else:
                    print(f"❌ Failed to clean build: {result.stderr.decode()}")
            except Exception as e:
                print(f"❌ Error while running make clean: {e}")
        else:
            print(f"⚠️ Build directory '{build_dir}' does not exist.")

    def check_devkitpro_installed(self):
        """
        Checks if DevKitPro is installed on the system.

        Returns:
            bool: True if DevKitPro is installed, False otherwise.
        """
        if self.runningSystem == "Windows":
            devkitpro = os.getenv('DEVKITPRO')
            devkita64 = os.getenv('DEVKITA64')

            if devkitpro and devkita64:
                print("✅ DevKitPro is installed on Windows!")
                return True
            else:
                print("❌ DevKitPro is not installed on Windows.")
                return False

        elif self.runningSystem == "Linux":
            devkitpro_dir = '/opt/devkitpro'

            if os.path.exists(devkitpro_dir) and os.getenv('DEVKITPRO'):
                print("✅ DevKitPro is installed on Linux!")
                return True
            else:
                print("❌ DevKitPro is not installed on Linux.")
                return False

        else:
            print("❌ Unsupported platform for DevKitPro check.")
            return False

    def buildExample(self, target, exampleName, debugging=False):
        """
        Builds an example project for the specified platform.

        Args:
            target (str): The target platform for the build (e.g., "Windows", "Linux", "Switch").
            exampleName (str): The name of the example project to build.
        """
        if not target:
            target = self.runningSystem

        if target not in VALID_BUILDING_PLATFORMS:
            print(f"❌ Unsupported platform: {target}")
            return

        frameworkPath = EXAMPLES_INTERNAL / exampleName
        if not frameworkPath.exists():
            print(f"❌ Example '{exampleName}' not found at {frameworkPath}")
            return

        build_path = frameworkPath / "build" / target
        build_path.mkdir(parents=True, exist_ok=True)

        # CMake Build
        print(f"🔧 Starting CMake build for {exampleName} on {target}...")
        result = subprocess.Popen([
            "cmake", "-DPLATFORM=" + target, "-G", "Unix Makefiles", str(frameworkPath.resolve())
        ], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, "Building CMake Configs")

        stdout, stderr = result.communicate()

        if result.returncode != 0:
            print("❌ CMake configuration failed.")
            print(f"⚠️ CMakeError: {stderr.decode()}")
            return

        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

        # Make Build
        print(f"🔧 Building {exampleName} with Make...")
        result = subprocess.Popen(["make"], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, f"Building {exampleName}")

        stdout, stderr = result.communicate()

        if result.returncode == 0:
            print("✅ Build successful!")
            self.copy_resources(build_path)
            self.collect_includes()
        else:
            print("❌ Build failed.")
            print(f"⚠️ MakeError: {stderr.decode()}")
        
        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

    def buildSanbox(self, target, debugging=False):
        """
        Builds the Sandbox project for the specified platform.

        Args:
            target (str): The target platform for the build (e.g., "Windows", "Linux", "Switch").
            exampleName (str): The name of the example project to build.
        """
        if not target:
            target = self.runningSystem

        if target not in VALID_BUILDING_PLATFORMS:
            print(f"❌ Unsupported platform: {target}")
            return

        frameworkPath = SANBOX_DIR_INTERNAL
        if not frameworkPath.exists():
            print(f"❌ Internal Sandbox Project not found.")
            return

        build_path = frameworkPath / "build" / target
        build_path.mkdir(parents=True, exist_ok=True)

        # CMake Build
        print(f"🔧 Starting CMake build for Sandbox project on {target}...")
        result = subprocess.Popen([
            "cmake", "-DPLATFORM=" + target, "-G", "Unix Makefiles", str(frameworkPath.resolve())
        ], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, "Building CMake Configs")

        stdout, stderr = result.communicate()

        if result.returncode != 0:
            print("❌ CMake configuration failed.")
            print(f"⚠️ CMakeError: {stderr.decode()}")
            return

        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

        # Make Build
        print(f"🔧 Building Sandbox with Make...")
        result = subprocess.Popen(["make"], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, f"Building Sandbox Project...")

        stdout, stderr = result.communicate()

        if result.returncode == 0:
            print("✅ Build successful!")
            self.copy_resources(build_path)
            self.collect_includes()
        else:
            print("❌ Build failed.")
            print(f"⚠️ MakeError: {stderr.decode()}")
        
        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())