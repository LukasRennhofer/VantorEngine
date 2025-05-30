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
 * File: vtrgCLIUtils.py
 * Last Change: Updated the projects build system
"""

import time
import sys

# ==================== ConsoleUtils: TODO =======================
class vtrgCLIClientUtils:
    # Colors
    colorGreen = "\033[32m"
    colorYellow = "\033[33m"
    colorPurple = "\033[0;35m"
    coloReset = "\033[0m"

    # ===== Output ======
    @staticmethod
    def show_loading_indicator(process, task_name):
        """
        Displays a loading indicator in the console while a subprocess is running.

        Args:
            process (subprocess.Popen): The subprocess to monitor.
            task_name (str): The name of the task being executed.
        """
        dots = 6
        while True:
            retcode = process.poll()
            if retcode is not None:
                if retcode == 0:
                    sys.stdout.write(f"\r{vtrgCLIClientUtils.colorGreen}✅ Complete!{" " * (len(task_name) + 5)}{vtrgCLIClientUtils.coloReset}\n")
                else:
                    sys.stdout.write(f"\r{vtrgCLIClientUtils.colorGreen}❌ Failed!{" " * (len(task_name) + 8)}{vtrgCLIClientUtils.coloReset}\n")
                break

            dot_count = int(time.time() * 10) % dots
            dots_str = " ".join([vtrgCLIClientUtils.colorPurple + "•" + vtrgCLIClientUtils.coloReset if j <= dot_count else "•" for j in range(dots)])
            sys.stdout.write(f"\r{vtrgCLIClientUtils.colorGreen}{task_name}...{vtrgCLIClientUtils.coloReset} [{dots_str}]")
            sys.stdout.flush()
            time.sleep(0.1)