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
try:
    from ..Console.vtrgCLIUtils import vtrgCLIClientUtils
    from ..core.logger import logger
except ImportError:
    # Fallback for legacy imports
    from Tools.vtrg.Console.vtrgCLIUtils import vtrgCLIClientUtils
    # Create a simple logger fallback
    class SimpleLogger:
        def info(self, msg): print(f"ℹ️ {msg}")
        def success(self, msg): print(f"✅ {msg}")
        def warning(self, msg): print(f"⚠️ {msg}")
        def error(self, msg): print(f"❌ {msg}")
        def debug(self, msg): pass
    logger = SimpleLogger()

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

    def check_dependencies(self):
        """
        Checks if Dependenices are avalible
        """
        try:
            result = subprocess.run(['cmake', '--version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            return result.returncode == 0
        except FileNotFoundError:
            return False

    def copy_resources(self, build_path):
        """
        Copies resource directories to the build path.
        """

        dest = build_path / Path("Resources")
        dest_shaders = build_path / Path("Shaders")

        # Private Resources
        if RESOURCE_DIR.exists():
            shutil.copytree(RESOURCE_DIR / Path("Private"), dest, dirs_exist_ok=True)
            print(f"📦 Copied Private Resources {RESOURCE_DIR / Path("Private")} to {dest}")
        else:
            print(f"⚠️ Private Resource folder not found: {RESOURCE_DIR / Path("Private")}")

        if SHADERS_DIR.exists():
            shutil.copytree(SHADERS_DIR, dest_shaders, dirs_exist_ok=True)
            print(f"📦 Copied Private Shaders {RESOURCE_DIR} to {dest_shaders}")
        else:
            print(f"⚠️ Private Shaders folder not found: {RESOURCE_DIR / Path("Private")}")

    def collect_includes(self):
        """
        NOT IN USE!!!: Collects all header files from the source directory and copies them to the include directory.
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
        NOT IN USE: Checks if DevKitPro is installed on the system.
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

    def buildIntern(self, debugging=False, target=None, module=None):
        """
        Builds the Vantor library or module for the specified platform.
        """
        if not target:
            target = self.runningSystem


        if target not in VALID_BUILDING_PLATFORMS:
            print(f"❌ Unsupported platform: {target}")
            return


        if not module:
            build_path = BUILD_DIR_INTERNAL / Path("Intern") / target
            build_path.mkdir(parents=True, exist_ok=True)

        else:
            build_path = BUILD_DIR_INTERNAL / Path("Module_" + module) / target
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

        vtrgCLIClientUtils.show_loading_indicator(result, "🔧 Building the library")

        stdout, stderr = result.communicate()

        if result.returncode == 0:
            print("✅ Build successful!")
            self.copy_resources(build_path)
        else:
            print("❌ Build failed.")
            print(f"⚠️ MakeError: {stderr.decode()}")

        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

    def buildProject(self, target, exampleName, debugging=False):
        """
        Builds an sample or sandbox project for the specified platform.
        """

        exepath = None

        if not target:
            target = self.runningSystem

        if target not in VALID_BUILDING_PLATFORMS:
            print(f"❌ Unsupported platform: {target}")
            return

        if exampleName == "Sandbox":
            frameworkPath = SANBOX_DIR_INTERNAL
            frametype = "sandbox"
            exepath = "VantorSandbox"

        elif exampleName in VALID_SAMPLES:
            frameworkPath = EXAMPLES_INTERNAL / Path(exampleName)
            frametype = "sample"
            exepath = exampleName

        else:
            print(f"❌ '{exampleName}' Sample or Sandbox target not found")

        if not frameworkPath.exists():
            print(f"❌ Sample or Sandbox '{exampleName}' not found at {frameworkPath}")
            return

        build_path = BUILD_DIR_INTERNAL / Path("Samples") / Path(exampleName)
        build_path.mkdir(parents=True, exist_ok=True)
        exepath = str(build_path / Path(exepath))

        # Cross Build Linux -> Windows
        if self.runningSystem == "Linux" and target == "Windows":
            toolchain_path = Path("/run/media/lukas/F0A8-AAEB/Vantor/Vantor/Vantor/CMake/VantorMingwToolchain.cmake")
            if not toolchain_path.exists():
                print(f"❌ Toolchain file not found at {toolchain_path}")
                return

            print(f"🔧 Starting CMake Cross Build build for {exampleName} on {target}...")
            # Include Toolchain file
            result = subprocess.Popen([
                "cmake", "-DPLATFORM=" + target, f"-DCMAKE_TOOLCHAIN_FILE={toolchain_path}", "-G", "Unix Makefiles", str(frameworkPath.resolve())
            ], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        else:
             # Native Build
            print(f"🔧 Starting CMake build for {exampleName} on {target}...")
            result = subprocess.Popen([
                "cmake", "-DPLATFORM=" + target, "-G", "Unix Makefiles", str(frameworkPath.resolve())
            ], cwd=build_path, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        vtrgCLIClientUtils.show_loading_indicator(result, "🔧 Building CMake Configs")

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
        else:
            print("❌ Build failed.")
            print(f"⚠️ MakeError: {stderr.decode()}")
        
        if debugging:
            print("[DEBUGGING] Resolved Output:", stdout.decode())

        return exepath

    def run_executable(self, executable_path: str, platform: str = None) -> int:
        """Run an executable file."""
        try:
            import subprocess
            import platform as plat
            
            if not os.path.exists(executable_path):
                logger.error(f"Executable not found: {executable_path}")
                return 1
            
            logger.info(f"Running executable: {executable_path}")
            
            # Handle cross-platform execution
            if platform == "Windows" and plat.system() == "Linux":
                # Try to run Windows executable with wine
                try:
                    result = subprocess.run(["wine", executable_path], check=True)
                    return result.returncode
                except FileNotFoundError:
                    logger.error("Wine not found. Cannot run Windows executable on Linux.")
                    return 1
            else:
                # Run native executable
                if not executable_path.startswith('./'):
                    executable_path = './' + executable_path
                result = subprocess.run([executable_path], check=True)
                return result.returncode
                
        except subprocess.CalledProcessError as e:
            logger.error(f"Executable failed with exit code {e.returncode}")
            return e.returncode
        except Exception as e:
            logger.error(f"Failed to run executable: {e}")
            return 1
    
    def clean_all(self, dry_run: bool = False) -> bool:
        """Clean all build artifacts and caches."""
        try:
            success = True
            
            # Clean build directories
            build_dirs = ["Build", "build", "CMakeFiles"]
            for build_dir in build_dirs:
                if os.path.exists(build_dir):
                    if dry_run:
                        logger.info(f"Would remove: {build_dir}")
                    else:
                        shutil.rmtree(build_dir)
                        logger.info(f"Removed: {build_dir}")
            
            # Clean cache files
            cache_patterns = ["*.cache", "__pycache__", "*.pyc"]
            for root, dirs, files in os.walk("."):
                for pattern in cache_patterns:
                    if pattern in dirs:
                        cache_path = os.path.join(root, pattern)
                        if dry_run:
                            logger.info(f"Would remove: {cache_path}")
                        else:
                            shutil.rmtree(cache_path)
                            logger.info(f"Removed: {cache_path}")
                
                for file in files:
                    for pattern in cache_patterns:
                        if file.endswith(pattern.replace("*", "")):
                            file_path = os.path.join(root, file)
                            if dry_run:
                                logger.info(f"Would remove: {file_path}")
                            else:
                                os.remove(file_path)
                                logger.info(f"Removed: {file_path}")
            
            return success
            
        except Exception as e:
            logger.error(f"Failed to clean all: {e}")
            return False
    
    def clean_build_dirs(self, dry_run: bool = False) -> bool:
        """Clean only build directories."""
        try:
            build_dirs = ["Build", "build", "CMakeFiles"]
            for build_dir in build_dirs:
                if os.path.exists(build_dir):
                    if dry_run:
                        logger.info(f"Would remove build directory: {build_dir}")
                    else:
                        shutil.rmtree(build_dir)
                        logger.info(f"Removed build directory: {build_dir}")
            return True
        except Exception as e:
            logger.error(f"Failed to clean build directories: {e}")
            return False
    
    def clean_cache(self, dry_run: bool = False) -> bool:
        """Clean cache files."""
        try:
            cache_patterns = ["__pycache__", "*.pyc", "*.cache"]
            for root, dirs, files in os.walk("."):
                # Remove cache directories
                dirs_to_remove = [d for d in dirs if d in cache_patterns]
                for cache_dir in dirs_to_remove:
                    cache_path = os.path.join(root, cache_dir)
                    if dry_run:
                        logger.info(f"Would remove cache directory: {cache_path}")
                    else:
                        shutil.rmtree(cache_path)
                        logger.info(f"Removed cache directory: {cache_path}")
                
                # Remove cache files
                for file in files:
                    if file.endswith('.pyc') or file.endswith('.cache'):
                        file_path = os.path.join(root, file)
                        if dry_run:
                            logger.info(f"Would remove cache file: {file_path}")
                        else:
                            os.remove(file_path)
                            logger.info(f"Removed cache file: {file_path}")
            return True
        except Exception as e:
            logger.error(f"Failed to clean cache: {e}")
            return False