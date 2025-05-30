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
 *  File: vantorPlatformSwitch.cpp
 *  Last Change: Automatically updated
 */

#ifdef __SWITCH__

#include "vantorPlatformSwitch.hpp"
#include <switch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <cstring>

namespace vantor::Platform
{
    double PlatformSwitch::startTime = 0.0;

    void PlatformSwitch::Initialize()
    {
        socketInitializeDefault();
        romfsInit();
        startTime = GetTimeSeconds();
    }

    double PlatformSwitch::GetTimeSeconds()
    {
        TimeSpan ts;
        timeGetCurrentTime(TimeType_UserSystemClock, &ts);
        return static_cast<double>(ts.nanoseconds) / 1'000'000'000.0;
    }

    void PlatformSwitch::SleepMilliseconds(unsigned int ms) { svcSleepThread(static_cast<int64_t>(ms) * 1'000'000); }

    std::string PlatformSwitch::GetExecutablePath()
    {
        return "romfs:/"; // Default on homebrew
    }

    std::string PlatformSwitch::GetExecutableDirectory()
    {
        return "romfs:/"; // Safe fallback
    }

    bool PlatformSwitch::FileExists(const std::string &path)
    {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    uint64_t PlatformSwitch::GetFileSize(const std::string &path)
    {
        struct stat st;
        if (stat(path.c_str(), &st) == 0) return static_cast<uint64_t>(st.st_size);
        return 0;
    }
} // namespace vantor::Platform

#endif // __SWITCH__
