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
 * File: logger.py
 * Description: Enhanced logging system for VTRG tool
"""

import logging
import sys
from datetime import datetime
from enum import Enum
from typing import Optional


class LogLevel(Enum):
    """Log levels with associated colors and icons."""
    DEBUG = ("DEBUG", "🔍", "\033[37m")     # White
    INFO = ("INFO", "ℹ️", "\033[36m")        # Cyan
    SUCCESS = ("SUCCESS", "✅", "\033[32m")  # Green
    WARNING = ("WARNING", "⚠️", "\033[33m")  # Yellow
    ERROR = ("ERROR", "❌", "\033[31m")      # Red
    CRITICAL = ("CRITICAL", "💥", "\033[35m") # Magenta


class VTRGLogger:
    """Enhanced logger for VTRG with colors and icons."""
    
    RESET_COLOR = "\033[0m"
    
    def __init__(self, name: str = "VTRG", verbose: bool = False):
        self.name = name
        self.verbose = verbose
        self.logger = logging.getLogger(name)
        self.logger.setLevel(logging.DEBUG if verbose else logging.INFO)
        
        # Remove existing handlers
        for handler in self.logger.handlers[:]:
            self.logger.removeHandler(handler)
        
        # Create console handler
        handler = logging.StreamHandler(sys.stdout)
        handler.setFormatter(self._create_formatter())
        self.logger.addHandler(handler)
    
    def _create_formatter(self) -> logging.Formatter:
        """Create a custom formatter."""
        return logging.Formatter(
            fmt='%(message)s',
            datefmt='%H:%M:%S'
        )
    
    def _log(self, level: LogLevel, message: str, timestamp: bool = True):
        """Internal logging method."""
        level_name, icon, color = level.value
        reset = self.RESET_COLOR
        
        if timestamp:
            time_str = datetime.now().strftime("%H:%M:%S")
            formatted_message = f"{color}[{time_str}] {icon} {message}{reset}"
        else:
            formatted_message = f"{color}{icon} {message}{reset}"
        
        # Map to standard logging levels
        log_level_map = {
            LogLevel.DEBUG: logging.DEBUG,
            LogLevel.INFO: logging.INFO,
            LogLevel.SUCCESS: logging.INFO,
            LogLevel.WARNING: logging.WARNING,
            LogLevel.ERROR: logging.ERROR,
            LogLevel.CRITICAL: logging.CRITICAL,
        }
        
        self.logger.log(log_level_map[level], formatted_message)
    
    def debug(self, message: str):
        """Log debug message."""
        if self.verbose:
            self._log(LogLevel.DEBUG, message)
    
    def info(self, message: str):
        """Log info message."""
        self._log(LogLevel.INFO, message)
    
    def success(self, message: str):
        """Log success message."""
        self._log(LogLevel.SUCCESS, message)
    
    def warning(self, message: str):
        """Log warning message."""
        self._log(LogLevel.WARNING, message)
    
    def error(self, message: str):
        """Log error message."""
        self._log(LogLevel.ERROR, message)
    
    def critical(self, message: str):
        """Log critical message."""
        self._log(LogLevel.CRITICAL, message)
    
    def header(self, title: str):
        """Print a formatted header."""
        border = "=" * 60
        print(f"\n{LogLevel.INFO.value[2]}{border}")
        print(f"  {title.upper()}")
        print(f"{border}{self.RESET_COLOR}\n")
    
    def section(self, title: str):
        """Print a section separator."""
        border = "-" * 40
        print(f"\n{LogLevel.INFO.value[2]}{border}")
        print(f"  {title}")
        print(f"{border}{self.RESET_COLOR}")


# Global logger instance
logger = VTRGLogger()


def set_verbose(verbose: bool):
    """Set global verbose mode."""
    global logger
    logger = VTRGLogger(verbose=verbose)
