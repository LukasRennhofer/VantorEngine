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
 * File: commands.py
 * Description: Command handlers for VTRG CLI
"""

from abc import ABC, abstractmethod
from typing import Any, Dict
import argparse

from ..core.logger import logger
from ..core.exceptions import VTRGError


class Command(ABC):
    """Base class for all VTRG commands."""
    
    @property
    @abstractmethod
    def name(self) -> str:
        """Command name."""
        pass
    
    @property
    @abstractmethod
    def help(self) -> str:
        """Command help text."""
        pass
    
    @abstractmethod
    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        """Add command-specific arguments to the parser."""
        pass
    
    @abstractmethod
    def execute(self, args: argparse.Namespace) -> int:
        """Execute the command. Return 0 for success, non-zero for failure."""
        pass


class BuildCommand(Command):
    """Build command for compiling projects and libraries."""
    
    @property
    def name(self) -> str:
        return "build"
    
    @property
    def help(self) -> str:
        return "Build internal modules, samples, or the sandbox"
    
    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            '--sample', 
            type=str, 
            help='Build a specific sample project from the Samples directory'
        )
        parser.add_argument(
            '--platform', 
            type=str, 
            choices=["Windows", "Linux", "Switch"], 
            help='Target platform for building'
        )
        parser.add_argument(
            '--clean', 
            type=str, 
            metavar='DIR',
            help="Clean specified directory before building"
        )
        parser.add_argument(
            '--debug', 
            action='store_true', 
            help="Enable debug build configuration"
        )
        parser.add_argument(
            '--release', 
            action='store_true', 
            help="Enable release build configuration (default)"
        )
        parser.add_argument(
            '--run', 
            action='store_true', 
            help="Run the executable immediately after building"
        )
        parser.add_argument(
            '--check-deps', 
            action='store_true', 
            help="Check if required dependencies are installed"
        )
        parser.add_argument(
            '--target',
            type=str,
            help="Specific build target"
        )
    
    def execute(self, args: argparse.Namespace) -> int:
        """Execute the build command."""
        try:
            # Handle imports gracefully
            try:
                from ..Building.vtrgBSystem import VantorInternalBuildSystem
            except ImportError:
                from Tools.vtrg.Building.vtrgBSystem import VantorInternalBuildSystem
            
            logger.header("Vantor Build System")
            
            build_system = VantorInternalBuildSystem()
            
            # Check dependencies if requested
            if args.check_deps:
                logger.info("Checking build dependencies...")
                if not build_system.check_dependencies():
                    logger.error("Missing required dependencies")
                    return 1
                logger.success("All dependencies are available")
            
            # Clean if requested
            if args.clean:
                logger.info(f"Cleaning build directory: {args.clean}")
                build_system.cleanBuildDir(args.clean)
                logger.success("Build directory cleaned")
            
            # Determine build configuration
            debug_mode = args.debug
            if args.release and args.debug:
                logger.warning("Both --debug and --release specified, using debug mode")
            elif args.release:
                debug_mode = False
            
            # Build project
            logger.info("Starting build process...")
            
            if args.sample:
                executable_path = build_system.buildProject(
                    target=args.platform,
                    exampleName=args.sample,
                    debugging=debug_mode
                )
                
                if args.run and executable_path:
                    logger.info(f"Running executable: {executable_path}")
                    return build_system.run_executable(executable_path, args.platform) if hasattr(build_system, 'run_executable') else 0
            else:
                build_system.buildIntern(
                    target=args.platform,
                    debugging=debug_mode,
                    module=args.target
                )
            
            logger.success("Build completed successfully")
            return 0
            
        except VTRGError as e:
            logger.error(f"Build failed: {e}")
            return 1
        except Exception as e:
            logger.critical(f"Unexpected error during build: {e}")
            return 1


class FormatCommand(Command):
    """Format command for code formatting."""
    
    @property
    def name(self) -> str:
        return "format"
    
    @property
    def help(self) -> str:
        return "Format source code files"
    
    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            '--path',
            type=str,
            help='Specific path to format (default: entire source directory)'
        )
        parser.add_argument(
            '--check',
            action='store_true',
            help='Check formatting without making changes'
        )
        parser.add_argument(
            '--extensions',
            nargs='+',
            default=['cpp', 'hpp', 'c', 'h'],
            help='File extensions to format (default: cpp hpp c h)'
        )
        parser.add_argument(
            '--exclude',
            nargs='+',
            help='Patterns to exclude from formatting'
        )
    
    def execute(self, args: argparse.Namespace) -> int:
        """Execute the format command."""
        try:
            # Handle imports gracefully
            try:
                from ..Formatting.vtrgFormat import format_files_in_folder
                from ..Misc.vtrgConstants import SRC_DIR_INTERNAL
            except ImportError:
                # Fallback for legacy imports
                from Tools.vtrg.Formatting.vtrgFormat import format_files_in_folder
                from Tools.vtrg.Misc.vtrgConstants import SRC_DIR_INTERNAL
            
            logger.header("Code Formatting")
            
            target_path = args.path or str(SRC_DIR_INTERNAL)
            
            if args.check:
                logger.info(f"Checking code formatting in: {target_path}")
            else:
                logger.info(f"Formatting source code in: {target_path}")
            
            result = format_files_in_folder(
                target_path,
                check_only=args.check,
                extensions=args.extensions,
                exclude_patterns=args.exclude
            )
            
            if result:
                logger.success("Code formatting completed successfully")
                return 0
            else:
                logger.error("Code formatting failed or found issues")
                return 1
                
        except Exception as e:
            logger.error(f"Formatting failed: {e}")
            return 1


class CleanCommand(Command):
    """Clean command for removing build artifacts."""
    
    @property
    def name(self) -> str:
        return "clean"
    
    @property
    def help(self) -> str:
        return "Clean build artifacts and temporary files"
    
    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            '--all',
            action='store_true',
            help='Clean all build directories and caches'
        )
        parser.add_argument(
            '--build',
            action='store_true',
            help='Clean build directories'
        )
        parser.add_argument(
            '--cache',
            action='store_true',
            help='Clean cache files'
        )
        parser.add_argument(
            '--path',
            type=str,
            help='Specific path to clean'
        )
        parser.add_argument(
            '--dry-run',
            action='store_true',
            help='Show what would be cleaned without actually cleaning'
        )
    
    def execute(self, args: argparse.Namespace) -> int:
        """Execute the clean command."""
        try:
            # Handle imports gracefully
            try:
                from ..Building.vtrgBSystem import VantorInternalBuildSystem
            except ImportError:
                from Tools.vtrg.Building.vtrgBSystem import VantorInternalBuildSystem
            
            logger.header("Cleanup")
            
            build_system = VantorInternalBuildSystem()
            
            if args.dry_run:
                logger.info("Dry run mode - showing what would be cleaned:")
            
            if args.all or (not args.build and not args.cache and not args.path):
                # Clean everything
                result = build_system.clean_all(dry_run=args.dry_run)
            elif args.build:
                result = build_system.clean_build_dirs(dry_run=args.dry_run)
            elif args.cache:
                result = build_system.clean_cache(dry_run=args.dry_run)
            elif args.path:
                result = build_system.cleanBuildDir(args.path, dry_run=args.dry_run)
            else:
                logger.error("No cleanup option specified")
                return 1
            
            if result:
                logger.success("Cleanup completed successfully")
                return 0
            else:
                logger.error("Cleanup failed")
                return 1
                
        except Exception as e:
            logger.error(f"Cleanup failed: {e}")
            return 1


# Registry of all available commands
COMMANDS = [
    BuildCommand(),
    CleanCommand(),
]
