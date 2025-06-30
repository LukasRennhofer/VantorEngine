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
 *  Date: 2025-06-30
 *
 *  File: VPL_NSX.hpp
 *  Last Change: Automatically updated
 */

// Only include this if its building for Nintendo Switch !

#pragma once

#ifdef __NSX__

// Core
#include "../../Core/BackLog/VCO_Backlog.hpp"

#include <string>
#include <cstdint>
#include <vector>

namespace Vantor::Platform
{
    class PlatformNSX
    {
        public:
            static void Initialize();

            static double GetTimeSeconds();
            static void   SleepMilliseconds(unsigned int ms);

            static std::string GetExecutablePath();      // Not usually needed on NSX
            static std::string GetExecutableDirectory(); // Will be placeholder
            static bool        FileExists(const std::string &path);
            static uint64_t    GetFileSize(const std::string &path);

            static std::vector<uint8_t> ReadBinary(const std::string &path);
            static std::string          ReadText(const std::string &path);

        private:
            static double startTime;
    };
} // namespace Vantor::Platform

#endif // __NSX__