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
 * File: parser.py
 * Description: Enhanced argument parser for VTRG CLI
"""

import argparse
import sys
from typing import List, Optional

from .commands import COMMANDS, Command
from ..core.logger import logger, set_verbose
from ..Misc.vtrgConstants import ENGINE_NAME, MAIN_AUTHOR, COPYRIGHT_YEAR


class VTRGArgumentParser:
    """Enhanced argument parser for VTRG with better help and error handling."""
    
    def __init__(self):
        self.parser = self._create_parser()
        self.commands = {cmd.name: cmd for cmd in COMMANDS}
    
    def _create_parser(self) -> argparse.ArgumentParser:
        """Create the main argument parser."""
        parser = argparse.ArgumentParser(
            prog='vtrg',
            description=f'{ENGINE_NAME} Trigger - Development Tool for {ENGINE_NAME} Engine',
            epilog=f'Copyright © {COPYRIGHT_YEAR} {MAIN_AUTHOR}. Licensed under GPL v3.',
            formatter_class=argparse.RawDescriptionHelpFormatter,
            add_help=False
        )
        
        # Global options
        parser.add_argument(
            '-h', '--help',
            action='store_true',
            help='Show this help message and exit'
        )
        parser.add_argument(
            '-v', '--verbose',
            action='store_true',
            help='Enable verbose output'
        )
        parser.add_argument(
            '--version',
            action='store_true',
            help='Show version information'
        )
        parser.add_argument(
            '--no-color',
            action='store_true',
            help='Disable colored output'
        )
        
        # Subcommands
        subparsers = parser.add_subparsers(
            dest='command',
            title='Available Commands',
            description='Use "vtrg <command> --help" for more information about a command.',
            help='Command to execute'
        )
        
        # Add command parsers
        for command in COMMANDS:
            cmd_parser = subparsers.add_parser(
                command.name,
                help=command.help,
                formatter_class=argparse.RawDescriptionHelpFormatter
            )
            command.add_arguments(cmd_parser)
            cmd_parser.set_defaults(command_obj=command)
        
        return parser
    
    def parse_args(self, args: Optional[List[str]] = None) -> argparse.Namespace:
        """Parse command line arguments with enhanced error handling."""
        try:
            parsed_args = self.parser.parse_args(args)
            
            # Handle global options
            if parsed_args.help:
                self._show_help(parsed_args.command)
                sys.exit(0)
            
            if parsed_args.version:
                self._show_version()
                sys.exit(0)
            
            if parsed_args.verbose:
                set_verbose(True)
            
            if parsed_args.no_color:
                # TODO: Implement color disabling
                pass
            
            return parsed_args
            
        except SystemExit as e:
            # argparse calls sys.exit on error
            if e.code != 0:
                logger.error("Invalid command line arguments")
            raise
    
    def _show_help(self, command: Optional[str] = None):
        """Show enhanced help information."""
        if command and command in self.commands:
            # Show command-specific help
            self.parser.parse_args([command, '--help'])
        else:
            # Show general help with examples
            self._show_general_help()
    
    def _show_general_help(self):
        """Show enhanced general help with examples."""
        print(f"""
{ENGINE_NAME} Trigger (vtrg) - Development Tool for {ENGINE_NAME} Engine

USAGE:
    vtrg [OPTIONS] <COMMAND> [COMMAND_OPTIONS]

GLOBAL OPTIONS:
    -h, --help          Show this help message
    -v, --verbose       Enable verbose output
    --version          Show version information
    --no-color         Disable colored output

COMMANDS:
    build              Build internal modules, samples, or the sandbox
    format             Format source code files
    clean              Clean build artifacts and temporary files

Use "vtrg <command> --help" for more information about a specific command.

Copyright © {COPYRIGHT_YEAR} {MAIN_AUTHOR}. Licensed under GPL v3.
        """.strip())
    
    def _show_version(self):
        """Show version information."""
        print(f"""
{ENGINE_NAME} Trigger (vtrg) v1.0.0

Development tool for {ENGINE_NAME} Engine
Copyright © {COPYRIGHT_YEAR} {MAIN_AUTHOR}
Licensed under the GNU General Public License, Version 3

Platform Support: Windows, Linux, Nintendo Switch
        """.strip())
    
    def get_command(self, args: argparse.Namespace) -> Optional[Command]:
        """Get the command object from parsed arguments."""
        return getattr(args, 'command_obj', None)
