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
 * File: vtrgFormat.py
 * Description: Enhanced code formatting utilities for VTRG
"""

import os
import sys
import re
import subprocess
from pathlib import Path
from typing import List, Optional, Set
import fnmatch

from ..Misc.vtrgConstants import HEADER_STYLE
from ..core.logger import logger
from ..core.exceptions import FileSystemError


class CodeFormatter:
    """Enhanced code formatter with multiple formatting tools support."""
    
    SUPPORTED_EXTENSIONS = {'.cpp', '.hpp', '.c', '.h', '.cc', '.cxx', '.hxx'}
    INCLUDE_PATTERN = re.compile(r'^\s*#\s*include\s+[<"].+[>"].*$', re.MULTILINE)
    
    def __init__(self):
        """Initialize the code formatter."""
        self.available_formatters = self._detect_formatters()
    
    def _detect_formatters(self) -> List[str]:
        """Detect available code formatters on the system."""
        formatters = []
        
        # Check for clang-format
        try:
            result = subprocess.run(['clang-format', '--version'], 
                                  capture_output=True, text=True)
            if result.returncode == 0:
                formatters.append('clang-format')
                logger.debug(f"Found clang-format: {result.stdout.strip()}")
        except FileNotFoundError:
            pass
        
        if not formatters:
            logger.warning("No code formatters found. Only header formatting will be available.")
        
        return formatters
    
    def format_file(self, file_path: Path, check_only: bool = False) -> bool:
        """
        Format a single file.
        
        Args:
            file_path: Path to the file to format
            check_only: If True, only check formatting without making changes
            
        Returns:
            True if formatting was successful or file was already formatted
        """
        try:
            if not file_path.exists():
                logger.error(f"File does not exist: {file_path}")
                return False
            
            if file_path.suffix not in self.SUPPORTED_EXTENSIONS:
                logger.debug(f"Skipping unsupported file: {file_path}")
                return True
            
            original_content = file_path.read_text(encoding='utf-8')
            
            # Apply header formatting
            formatted_content = self._format_header(original_content, file_path.name)
            
            # Apply clang-format if available
            if 'clang-format' in self.available_formatters:
                formatted_content = self._apply_clang_format(formatted_content, file_path, check_only)
                if formatted_content is None:
                    return False
            
            # Sort includes
            formatted_content = self._sort_includes(formatted_content)
            
            if check_only:
                if original_content != formatted_content:
                    logger.warning(f"File needs formatting: {file_path}")
                    return False
                else:
                    logger.debug(f"File is properly formatted: {file_path}")
                    return True
            else:
                if original_content != formatted_content:
                    file_path.write_text(formatted_content, encoding='utf-8')
                    logger.info(f"Formatted: {file_path}")
                else:
                    logger.debug(f"No changes needed: {file_path}")
                return True
                
        except Exception as e:
            logger.error(f"Error formatting file {file_path}: {e}")
            return False
    
    def _apply_clang_format(self, content: str, file_path: Path, check_only: bool = False) -> Optional[str]:
        """Apply clang-format to the content."""
        try:
            if check_only:
                # Use --dry-run or check output differences
                result = subprocess.run(
                    ['clang-format', '--style=file', str(file_path)],
                    input=content,
                    capture_output=True,
                    text=True
                )
            else:
                result = subprocess.run(
                    ['clang-format', '--style=file'],
                    input=content,
                    capture_output=True,
                    text=True
                )
            
            if result.returncode != 0:
                logger.error(f"clang-format error: {result.stderr}")
                return None
            
            return result.stdout
            
        except Exception as e:
            logger.error(f"Error running clang-format: {e}")
            return None
    
    def _format_header(self, content: str, filename: str) -> str:
        """Insert or update the file header."""
        # Remove existing header
        header_pattern = re.compile(r"^\s*/\*\n(?: \*.*\n)+ \*/\n*", re.MULTILINE)
        content = re.sub(header_pattern, "", content)
        
        # Insert new header
        header = HEADER_STYLE.replace("{FILENAME}", filename)
        return header.strip() + "\n\n" + content.lstrip()
    
    def _sort_includes(self, content: str) -> str:
        """Sort and organize include statements."""
        lines = content.splitlines()
        
        # Find include blocks
        include_blocks = []
        current_block = []
        in_include_block = False
        
        for i, line in enumerate(lines):
            stripped = line.strip()
            
            if stripped.startswith('#include'):
                if not in_include_block:
                    in_include_block = True
                    current_block = [(i, line)]
                else:
                    current_block.append((i, line))
            elif in_include_block and (stripped == '' or stripped.startswith('//')):
                # Continue the block for empty lines and comments
                current_block.append((i, line))
            else:
                if in_include_block:
                    include_blocks.append(current_block)
                    current_block = []
                    in_include_block = False
        
        if current_block:
            include_blocks.append(current_block)
        
        # Sort each include block
        for block in include_blocks:
            includes = []
            other_lines = []
            
            for line_num, line in block:
                if line.strip().startswith('#include'):
                    includes.append(line)
                else:
                    other_lines.append((line_num, line))
            
            # Separate system and local includes
            system_includes = [inc for inc in includes if '<' in inc]
            local_includes = [inc for inc in includes if '"' in inc]
            
            # Sort each group
            system_includes.sort()
            local_includes.sort()
            
            # Replace the block in the original lines
            sorted_includes = system_includes + ([''] if system_includes and local_includes else []) + local_includes
            
            # Update the lines
            start_idx = block[0][0]
            for i, new_line in enumerate(sorted_includes):
                if start_idx + i < len(lines):
                    lines[start_idx + i] = new_line
        
        return '\n'.join(lines)
    
    def format_directory(self, directory: Path, extensions: List[str] = None, 
                        exclude_patterns: List[str] = None, check_only: bool = False) -> bool:
        """
        Format all files in a directory recursively.
        
        Args:
            directory: Directory to format
            extensions: File extensions to format (default: all supported)
            exclude_patterns: Patterns to exclude from formatting
            check_only: If True, only check formatting without making changes
            
        Returns:
            True if all files were formatted successfully
        """
        if not directory.exists():
            logger.error(f"Directory does not exist: {directory}")
            return False
        
        extensions = extensions or [ext[1:] for ext in self.SUPPORTED_EXTENSIONS]  # Remove dots
        exclude_patterns = exclude_patterns or ['External/', 'Build/', '__pycache__/']
        
        success = True
        file_count = 0
        
        for root, dirs, files in os.walk(directory):
            # Remove excluded directories
            dirs[:] = [d for d in dirs if not any(fnmatch.fnmatch(d, pattern.rstrip('/')) for pattern in exclude_patterns)]
            
            for file_name in files:
                # Check extension
                if not any(file_name.endswith(f'.{ext}') for ext in extensions):
                    continue
                
                file_path = Path(root) / file_name
                
                # Check exclude patterns
                relative_path = file_path.relative_to(directory)
                if any(fnmatch.fnmatch(str(relative_path), pattern) for pattern in exclude_patterns):
                    continue
                
                file_count += 1
                if not self.format_file(file_path, check_only):
                    success = False
        
        action = "checked" if check_only else "formatted"
        logger.info(f"Processed {file_count} files in {directory}")
        
        return success


# Global formatter instance
_formatter = CodeFormatter()


def format_files_in_folder(folder_path: str, check_only: bool = False, 
                         extensions: List[str] = None, exclude_patterns: List[str] = None) -> bool:
    """
    Format files in a folder (legacy function for backwards compatibility).
    
    Args:
        folder_path: Path to the folder to format
        check_only: If True, only check formatting without making changes
        extensions: File extensions to format
        exclude_patterns: Patterns to exclude from formatting
        
    Returns:
        True if formatting was successful
    """
    return _formatter.format_directory(
        Path(folder_path), 
        extensions=extensions, 
        exclude_patterns=exclude_patterns,
        check_only=check_only
    )


def format_cpp_file(file_path: str) -> bool:
    """Format a single C++ file (legacy function for backwards compatibility)."""
    return _formatter.format_file(Path(file_path))


def formatHeaderinfile(path: str) -> bool:
    """Format header in a file (legacy function for backwards compatibility)."""
    return _formatter.format_file(Path(path))
