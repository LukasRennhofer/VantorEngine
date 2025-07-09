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
 * File: main.py
 * Description: Enhanced main entry point for VTRG CLI tool
"""

import sys
import signal
from typing import Optional

from .cli.parser import VTRGArgumentParser
from .core.logger import logger
from .core.exceptions import VTRGError


class VTRGApplication:
    """Main VTRG application class."""
    
    def __init__(self):
        """Initialize the VTRG application."""
        self.parser = VTRGArgumentParser()
        
        # Setup signal handlers for graceful shutdown
        signal.signal(signal.SIGINT, self._signal_handler)
        signal.signal(signal.SIGTERM, self._signal_handler)
    
    def _signal_handler(self, signum: int, frame) -> None:
        """Handle interrupt signals gracefully."""
        logger.warning("\nOperation interrupted by user")
        sys.exit(130)  # Standard exit code for SIGINT
    
    def run(self, args: Optional[list] = None) -> int:
        """
        Run the VTRG application.
        
        Args:
            args: Command line arguments (defaults to sys.argv[1:])
            
        Returns:
            Exit code (0 for success, non-zero for failure)
        """
        try:
            # Parse command line arguments
            parsed_args = self.parser.parse_args(args)
            
            # Check if a command was provided
            if not hasattr(parsed_args, 'command') or parsed_args.command is None:
                logger.error("No command specified. Use 'vtrg --help' for usage information.")
                return 1
            
            # Get the command object
            command = self.parser.get_command(parsed_args)
            if command is None:
                logger.error(f"Unknown command: {parsed_args.command}")
                return 1
            
            # Execute the command
            logger.debug(f"Executing command: {command.name}")
            return command.execute(parsed_args)
            
        except KeyboardInterrupt:
            logger.warning("\nOperation interrupted by user")
            return 130
        except VTRGError as e:
            logger.error(f"VTRG Error: {e}")
            return 1
        except Exception as e:
            logger.critical(f"Unexpected error: {e}")
            logger.debug("Use --verbose for detailed error information")
            return 1
    
    def version_check(self) -> bool:
        """Check if the tool is up to date."""
        # TODO: Implement version checking
        return True


def main() -> int:
    """Main entry point for the VTRG tool."""
    app = VTRGApplication()
    return app.run()


if __name__ == "__main__":
    sys.exit(main())
