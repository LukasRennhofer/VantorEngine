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
 * File: chifPlatformSwitch.cpp
 * Last Change: 
*/

#ifdef __SWITCH__

#include "chifPlatformSwitch.hpp"
#include <switch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <cstring>

namespace chif::Platform
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

    void PlatformSwitch::SleepMilliseconds(unsigned int ms)
    {
        svcSleepThread(static_cast<int64_t>(ms) * 1'000'000);
    }

    std::string PlatformSwitch::GetExecutablePath()
    {
        return "romfs:/"; // Default on homebrew
    }

    std::string PlatformSwitch::GetExecutableDirectory()
    {
        return "romfs:/"; // Safe fallback
    }

    bool PlatformSwitch::FileExists(const std::string& path)
    {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    uint64_t PlatformSwitch::GetFileSize(const std::string& path)
    {
        struct stat st;
        if (stat(path.c_str(), &st) == 0)
            return static_cast<uint64_t>(st.st_size);
        return 0;
    }
}

#endif // __SWITCH__
