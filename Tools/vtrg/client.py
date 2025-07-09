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
 * File: client.py
 * Description: Legacy client wrapper for backwards compatibility
"""

import sys
from .main import main

# Legacy class for backwards compatibility
class vtrgCLIClient:
    """Legacy wrapper class for backwards compatibility."""
    
    def __init__(self):
        """Initialize the legacy client."""
        pass
    
    def execute(self):
        """Execute the VTRG tool using the new main entry point."""
        return main()

# For backwards compatibility, if this file is run directly
if __name__ == "__main__":
    sys.exit(main())