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
 * File: exceptions.py
 * Description: Custom exceptions for VTRG tool
"""

class VTRGError(Exception):
    """Base exception for VTRG tool."""
    pass


class BuildError(VTRGError):
    """Exception raised when build operations fail."""
    pass


class DependencyError(VTRGError):
    """Exception raised when dependencies are missing."""
    pass


class PlatformError(VTRGError):
    """Exception raised for platform-specific errors."""
    pass


class ConfigurationError(VTRGError):
    """Exception raised for configuration errors."""
    pass


class FileSystemError(VTRGError):
    """Exception raised for file system operations errors."""
    pass
