/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifPlatformEnvironment.hpp
 * Last Change: 
*/

/*
This file is meant for connecting the current used platform and a initializing its core systems such as time and the filesystem
e.g.:
    chif::Platform::Environment::Initialize();
*/

#pragma once

#include <string>
#include <cstdint>

#if defined(__WINDOWS__)
    #include "chifPlatformWindows.hpp"
    using PlatformImpl = chif::Platform::PlatformWindows;
#elif defined(__SWITCH__)
    #include "chifPlatformSwitch.hpp"
    using PlatformImpl = chif::Platform::PlatformSwitch;
#elif defined(__LINUX__)
    #include "chifPlatformLinux.hpp"
    using PlatformImpl = chif::Platform::PlatformLinux;
#endif

namespace chif::Platform
{
    namespace Environment
    {
        enum Platforms {
            WINDOWS,
            LINUX,
            SWITCH
        };

        inline void Initialize()                          { PlatformImpl::Initialize(); }

        inline double GetTimeSeconds()                    { return PlatformImpl::GetTimeSeconds(); }
        inline void SleepMilliseconds(unsigned int ms)    { PlatformImpl::SleepMilliseconds(ms); }

        inline auto GetExecutablePath()                   { return PlatformImpl::GetExecutablePath(); }
        inline auto GetExecutableDirectory()              { return PlatformImpl::GetExecutableDirectory(); }

        inline bool FileExists(const auto& path)          { return PlatformImpl::FileExists(path); }
        inline uint64_t GetFileSize(const auto& path)     { return PlatformImpl::GetFileSize(path); }

        constexpr Platforms getCurrentPlatform() {
            #if defined(__WINDOWS__)
                return WINDOWS;
            #elif defined(__SWITCH__)
                return SWITCH;
            #elif defined(__LINUX__)
                return LINUX;
            #endif
        }

        inline std::string getPlatformName() {
            Platforms platform = getCurrentPlatform();

            switch(platform) {
                case WINDOWS: return "Windows";
                case LINUX: return "Linux";
                case SWITCH: return "Switch";
                default: return "None";

            }
        }

    }
}

