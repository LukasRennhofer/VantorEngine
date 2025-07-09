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
 * File: vtrgConfig.py
 * Description: Enhanced configuration management for VTRG
"""

import json
import os
from pathlib import Path
from typing import Any, Dict, Optional

from ..core.logger import logger
from ..core.exceptions import ConfigurationError


class VTRGConfig:
    """Enhanced configuration management for VTRG."""
    
    DEFAULT_CONFIG = {
        'build': {
            'default_platform': 'Linux',
            'default_build_type': 'Debug',
            'parallel_jobs': 'auto',
            'clean_before_build': False,
            'show_build_output': True,
        },
        'format': {
            'auto_format': False,
            'check_before_commit': True,
            'extensions': ['cpp', 'hpp', 'c', 'h'],
            'exclude_patterns': ['External/', 'Build/', '__pycache__/'],
        },
        'paths': {
            'build_dir': 'Build',
            'external_dir': 'External',
            'samples_dir': 'Samples',
        },
        'editor': {
            'preferred_editor': 'code',
            'open_build_results': False,
        },
        'general': {
            'check_updates': True,
            'send_analytics': False,
            'color_output': True,
        }
    }
    
    def __init__(self, config_path: Optional[str] = None):
        """Initialize configuration manager."""
        if config_path:
            self.config_path = Path(config_path)
        else:
            # Default config location
            home_dir = Path.home()
            self.config_path = home_dir / '.vtrg' / 'config.json'
        
        self.config_path.parent.mkdir(parents=True, exist_ok=True)
        self._config = self._load_config()
    
    def _load_config(self) -> Dict[str, Any]:
        """Load configuration from file."""
        if self.config_path.exists():
            try:
                with open(self.config_path, 'r') as f:
                    config = json.load(f)
                
                # Merge with defaults to ensure all keys exist
                return self._merge_configs(self.DEFAULT_CONFIG, config)
            except (json.JSONDecodeError, IOError) as e:
                logger.warning(f"Failed to load config from {self.config_path}: {e}")
                logger.info("Using default configuration")
        
        # Return default config
        return self.DEFAULT_CONFIG.copy()
    
    def _merge_configs(self, default: Dict, user: Dict) -> Dict:
        """Recursively merge user config with defaults."""
        result = default.copy()
        
        for key, value in user.items():
            if key in result and isinstance(result[key], dict) and isinstance(value, dict):
                result[key] = self._merge_configs(result[key], value)
            else:
                result[key] = value
        
        return result
    
    def _save_config(self) -> None:
        """Save configuration to file."""
        try:
            with open(self.config_path, 'w') as f:
                json.dump(self._config, f, indent=2, sort_keys=True)
        except IOError as e:
            raise ConfigurationError(f"Failed to save config to {self.config_path}: {e}")
    
    def get(self, key: str, default: Any = None) -> Any:
        """Get configuration value using dot notation (e.g., 'build.default_platform')."""
        keys = key.split('.')
        value = self._config
        
        try:
            for k in keys:
                value = value[k]
            return value
        except (KeyError, TypeError):
            if default is not None:
                return default
            raise ConfigurationError(f"Configuration key '{key}' not found")
    
    def set(self, key: str, value: Any) -> None:
        """Set configuration value using dot notation."""
        keys = key.split('.')
        config = self._config
        
        # Navigate to the parent of the target key
        for k in keys[:-1]:
            if k not in config:
                config[k] = {}
            elif not isinstance(config[k], dict):
                raise ConfigurationError(f"Cannot set '{key}': '{k}' is not a dictionary")
            config = config[k]
        
        # Set the value
        config[keys[-1]] = value
        self._save_config()
    
    def show_all(self) -> None:
        """Display all configuration values."""
        logger.info("Current VTRG Configuration:")
        self._print_config(self._config)
    
    def _print_config(self, config: Dict, prefix: str = "") -> None:
        """Recursively print configuration."""
        for key, value in config.items():
            full_key = f"{prefix}.{key}" if prefix else key
            
            if isinstance(value, dict):
                logger.info(f"  {full_key}:")
                self._print_config(value, full_key)
            else:
                logger.info(f"  {full_key} = {value}")
    
    def reset(self) -> None:
        """Reset configuration to defaults."""
        self._config = self.DEFAULT_CONFIG.copy()
        self._save_config()
    
    def validate(self) -> bool:
        """Validate current configuration."""
        try:
            # Check that required sections exist
            required_sections = ['build', 'format', 'paths', 'editor', 'general']
            for section in required_sections:
                if section not in self._config:
                    logger.error(f"Missing required configuration section: {section}")
                    return False
            
            # Validate specific values
            platform = self.get('build.default_platform')
            if platform not in ['Windows', 'Linux', 'Switch']:
                logger.error(f"Invalid default platform: {platform}")
                return False
            
            build_type = self.get('build.default_build_type')
            if build_type not in ['Debug', 'Release']:
                logger.error(f"Invalid default build type: {build_type}")
                return False
            
            logger.success("Configuration validation passed")
            return True
            
        except Exception as e:
            logger.error(f"Configuration validation failed: {e}")
            return False