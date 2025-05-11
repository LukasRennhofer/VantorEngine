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
 *  Date: 2025-05-11
 *
 *  File: vantorPlatformEnvironment.hpp
 *  Last Change: Automatically updated
 */

/*
This file is meant for connecting the current used platform and a initializing
its core systems such as time and the filesystem e.g.:
    vantor::Platform::Environment::Initialize();
*/

#pragma once

#include <string>
#include <cstdint>

#if defined(__WINDOWS__)
#include "vantorPlatformWindows.hpp"
using PlatformImpl = vantor::Platform::PlatformWindows;
#elif defined(__SWITCH__)
#include "vantorPlatformSwitch.hpp"
using PlatformImpl = vantor::Platform::PlatformSwitch;
#elif defined(__LINUX__)
#include "vantorPlatformLinux.hpp"
using PlatformImpl = vantor::Platform::PlatformLinux;
#endif

namespace vantor::Platform
{
    namespace Environment
    {
        enum Platforms
        {
            WINDOWS,
            LINUX,
            SWITCH
        };

        inline void Initialize() { PlatformImpl::Initialize(); }

        inline double GetTimeSeconds() { return PlatformImpl::GetTimeSeconds(); }
        inline void   SleepMilliseconds(unsigned int ms) { PlatformImpl::SleepMilliseconds(ms); }

        inline auto GetExecutablePath() { return PlatformImpl::GetExecutablePath(); }
        inline auto GetExecutableDirectory() { return PlatformImpl::GetExecutableDirectory(); }

        inline bool     FileExists(const auto &path) { return PlatformImpl::FileExists(path); }
        inline uint64_t GetFileSize(const auto &path) { return PlatformImpl::GetFileSize(path); }

        constexpr Platforms getCurrentPlatform()
        {
#if defined(__WINDOWS__)
            return WINDOWS;
#elif defined(__SWITCH__)
            return SWITCH;
#elif defined(__LINUX__)
            return LINUX;
#endif
        }

        inline std::string getPlatformName()
        {
            Platforms platform = getCurrentPlatform();

            switch (platform)
            {
                case WINDOWS:
                    return "Windows";
                case LINUX:
                    return "Linux";
                case SWITCH:
                    return "Switch";
                default:
                    return "None";
            }
        }

    } // namespace Environment
} // namespace vantor::Platform
