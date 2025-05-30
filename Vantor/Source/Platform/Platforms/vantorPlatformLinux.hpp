/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorPlatformLinux.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#ifdef __LINUX__

#include <string>
#include <cstdint>

namespace vantor::Platform
{
    class PlatformLinux
    {
        public:
            static void Initialize(); // Call once at startup

            static double GetTimeSeconds();
            static void   SleepMilliseconds(unsigned int ms);

            static std::string GetExecutablePath();
            static std::string GetExecutableDirectory();

            static bool     FileExists(const std::string &path);
            static uint64_t GetFileSize(const std::string &path);

        private:
            static void        CacheExecutablePath();
            static std::string cachedPath;
            static std::string cachedDir;
    };
} // namespace vantor::Platform

#endif // __LINUX__
