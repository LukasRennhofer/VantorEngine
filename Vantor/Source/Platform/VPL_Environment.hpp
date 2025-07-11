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
 *  Date: 2025-07-11
 *
 *  File: VPL_Environment.hpp
 *  Last Change: Automatically updated
 */

/*
This file is meant for connecting the current used platform and a initializing
its core systems such as time and the filesystem e.g.:
    Vantor::Platform::Environment::Initialize();
*/

#pragma once

#include <string>
#include <vector>

#if defined(__WINDOWS__)
#include "Platforms/VPL_Windows.hpp"
using PlatformImpl = Vantor::Platform::PlatformWindows;
#elif defined(__NSX__)
#include "Platforms/VPL_NSX.hpp"
using PlatformImpl = Vantor::Platform::PlatformNSX;
#elif defined(__LINUX__)
#include "Platforms/VPL_Linux.hpp"
using PlatformImpl = Vantor::Platform::PlatformLinux;
#endif

namespace Vantor::Platform
{
    namespace Environment
    {
        enum VEPlatforms
        {
            WINDOWS,
            LINUX,
            NSX // Nintendo Switch
        };

        inline void Initialize() { PlatformImpl::Initialize(); }

        inline double GetTimeSeconds() { return PlatformImpl::GetTimeSeconds(); }
        inline void   SleepMilliseconds(unsigned int ms) { PlatformImpl::SleepMilliseconds(ms); }

        inline auto GetExecutablePath() { return PlatformImpl::GetExecutablePath(); }
        inline auto GetExecutableDirectory() { return PlatformImpl::GetExecutableDirectory(); }

        inline bool     FileExists(const auto &path) { return PlatformImpl::FileExists(path); }
        inline uint64_t GetFileSize(const auto &path) { return PlatformImpl::GetFileSize(path); }

        inline std::vector<uint8_t> ReadBinary(const std::string &path) { return PlatformImpl::ReadBinary(path); }
        inline std::string          ReadText(const std::string &path) { return PlatformImpl::ReadText(path); }

        constexpr VEPlatforms GetCurrentPlatform()
        {
#if defined(__WINDOWS__)
            return WINDOWS;
#elif defined(__NSX__)
            return NSX;
#elif defined(__LINUX__)
            return LINUX;
#endif
        }

        inline std::string GetPlatformName()
        {
            VEPlatforms platform = GetCurrentPlatform();

            switch (platform)
            {
                case WINDOWS:
                    return "Windows";
                case LINUX:
                    return "Linux";
                case NSX:
                    return "Nintendo Switch";
                default:
                    return "None";
            }
        }

    } // namespace Environment
} // namespace Vantor::Platform