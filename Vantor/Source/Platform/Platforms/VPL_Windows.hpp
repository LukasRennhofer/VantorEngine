/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-01
 *
 *  File: VPL_Windows.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#ifdef __WINDOWS__

// Core
#include "../../Core/BackLog/VCO_Backlog.hpp"

#include <string>
#include <cstdint>
#include <vector>

namespace Vantor::Platform
{
    class PlatformWindows
    {
        public:
            static void Initialize(); // Call at app start

            static double GetTimeSeconds();
            static void   SleepMilliseconds(unsigned int ms);

            static std::wstring GetExecutablePath();
            static std::wstring GetExecutableDirectory();

            static bool     FileExists(const std::string &path);
            static uint64_t GetFileSize(const std::string &path);

            static std::vector<uint8_t> ReadBinary(const std::string &path);
            static std::string          ReadText(const std::string &path);

        private:
            static void         CacheExecutablePath();
            static std::wstring cachedPath;
            static std::wstring cachedDir;
    };
} // namespace Vantor::Platform

#endif // __WINDOWS__